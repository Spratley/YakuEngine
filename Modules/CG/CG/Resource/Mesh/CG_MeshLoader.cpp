#include "PCH/CG_PCH.h"
#include "CG_MeshLoader.h"

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/File/YK_IOFile.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "CG/IO/CG_GLTF.h"
#include "CG/Libraries/TinyGLTF/tiny_gltf_v3.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Mesh/CG_MeshFactory.h"
#include "CG/Resource/Mesh/CG_MeshLayout.h"

#include <charconv>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

CG_Mesh CG_MeshLoader::Load(YK_FilePath const& p_path)
{
    std::string_view const& pathExtension = p_path.Extension();
    if (pathExtension == "obj")
    {
        return LoadOBJ(p_path);
    }
    else if (pathExtension == "glb" || pathExtension == "gltf")
    {
        return LoadGLTF(p_path);
    }

    YK_LOG_ERROR_PARAM("Attempting to load a mesh with an unsupported format!\n{}", p_path.CString());
    return CG_Mesh();
}

// TODO: This needs SUPER improvement. It's not good as it stands
CG_Mesh CG_MeshLoader::LoadOBJ(YK_FilePath const& p_path)
{
    std::stringstream objFileContents;
    YK_IFile::GetFileContents(p_path.CString(), objFileContents);

    std::vector<YK_Vector3f> vertexPositions;
    std::vector<YK_Vector2f> vertexTexCoords;

    // Pair is the index of the vertex position and texture coords
    // Map here is just used to track which pairs have already been used so we can recycle GPU vertices
    std::map<std::tuple<YK_U32, YK_U32>, YK_U32> vertexTexturePairIndices;
    std::vector<float> vertices;
    std::vector<YK_U32> indices;

    std::string fileLine;
    while (std::getline(objFileContents, fileLine))
    {
        // TODO: Move string parsing to its own YKC file
        char const* dataPtr = &fileLine[fileLine.find_first_of(' ') + 1];
        char const* endPtr = fileLine.data() + fileLine.size();

        auto extractData = [&dataPtr, endPtr]<typename DataType>(DataType& outData) {
            auto [nextDataPtr, errorCode] = std::from_chars(dataPtr, endPtr, outData);
            dataPtr = nextDataPtr + 1;
        };

        std::string_view linePrefix(fileLine.data(), 2);
        if (linePrefix.compare("v ") == 0)
        {
            YK_Vector3f vertex;
            extractData(vertex.x);
            extractData(vertex.y);
            extractData(vertex.z);
            vertexPositions.push_back(vertex);
        }
        else if (linePrefix.compare("vt") == 0)
        {
            YK_Vector2f textureCoord;
            extractData(textureCoord.x);
            extractData(textureCoord.y);
            vertexTexCoords.push_back(textureCoord);
        }
        else if (linePrefix.compare("f ") == 0)
        {
            for (int i = 0; i < 3; ++i)
            {
                YK_U32 vertexPositionIndex;
                YK_U32 texCoordIndex;
                YK_U32 normalIndex;
                extractData(vertexPositionIndex);
                extractData(texCoordIndex);
                extractData(normalIndex);

                --vertexPositionIndex;
                --texCoordIndex;
                --normalIndex;

                auto const foundIndex =
                  vertexTexturePairIndices.find(std::make_tuple(vertexPositionIndex, texCoordIndex));
                if (foundIndex != vertexTexturePairIndices.end())
                {
                    indices.push_back(foundIndex->second);
                }
                else
                {
                    const YK_U32 vertexIndex = static_cast<YK_U32>(vertexTexturePairIndices.size());
                    vertexTexturePairIndices.emplace(
                      std::pair(std::make_tuple(vertexPositionIndex, texCoordIndex), vertexIndex));

                    YK_Vector3f const& vertexPosition = vertexPositions[vertexPositionIndex];
                    YK_Vector2f const& texCoord = vertexTexCoords[texCoordIndex];
                    vertices.push_back(vertexPosition.x);
                    vertices.push_back(vertexPosition.y);
                    vertices.push_back(vertexPosition.z);
                    vertices.push_back(texCoord.x);
                    vertices.push_back(texCoord.y);

                    indices.push_back(vertexIndex);
                }
            }
        }
    }

    // Right now this is hard-coded to ONLY load position and UV
    constexpr CG_MeshLayout layout{ CG_MeshAttribute::POSITION, CG_MeshAttribute::UV };

    return CG_MeshFactory::FromData(reinterpret_cast<YK_Byte const*>(vertices.data()),
                                    static_cast<YK_U32>(vertices.size()),
                                    indices.data(),
                                    static_cast<YK_U32>(indices.size()),
                                    layout);
}

CG_Mesh CG_MeshLoader::LoadGLTF(YK_FilePath const& p_path)
{
    CG_GLTF::File gltfMesh(p_path);
    if (gltfMesh.CheckErrors() || !gltfMesh.HasMesh())
    {
        return CG_Mesh();
    }

    tg3_model const& model = gltfMesh.GetModel();
    tg3_primitive const& primitive = model.meshes[0].primitives[0];

    // Joints are stored as 8 bit integers in GLTF exported from Blender, so we need a vector to store them
    using YK_Vector4U8 = YK_Vector_N<YK_U8, 4>;

    CG_MeshLayout layout;
    CG_GLTF::DataView<YK_Vector3f> vertices;
    CG_GLTF::DataView<YK_Vector3f> normals;
    CG_GLTF::DataView<YK_Vector2f> uvs;
    CG_GLTF::DataView<YK_Vector4U8> joints;
    CG_GLTF::DataView<YK_Vector4f> weights;

    for (auto i : YK_CountTo(primitive.attributes_count))
    {
        auto [name, accessorIndex] = primitive.attributes[i];

        if (name.len == 8 && std::strcmp(name.data, "POSITION") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::POSITION);
            vertices = gltfMesh.ViewData<YK_Vector3f>(accessorIndex);
        }
        else if (name.len == 6 && std::strcmp(name.data, "NORMAL") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::NORMAL);
            normals = gltfMesh.ViewData<YK_Vector3f>(accessorIndex);
        }
        else if (name.len == 10 && std::strcmp(name.data, "TEXCOORD_0") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::UV);
            uvs = gltfMesh.ViewData<YK_Vector2f>(accessorIndex);
        }
        else if (name.len == 8 && std::strcmp(name.data, "JOINTS_0") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::JOINT);
            joints = gltfMesh.ViewData<YK_Vector4U8>(accessorIndex);
        }
        else if (name.len == 9 && std::strcmp(name.data, "WEIGHTS_0") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::WEIGHT);
            weights = gltfMesh.ViewData<YK_Vector4f>(accessorIndex);
        }
    }

    // Joints needs 4x the contribution since we're increasing the size of each member to a full 32 bit integer
    YK_SizeT dataArrayByteCount =
      vertices.SizeBytes() + normals.SizeBytes() + uvs.SizeBytes() + (joints.SizeBytes() * 4) + weights.SizeBytes();

    std::vector<YK_Byte> interleavedData;
    interleavedData.resize(dataArrayByteCount);

    // Joints needs to manually offset by a Vector4
    YK_SizeT baseOffset = vertices.GetOffsetBytes() + normals.GetOffsetBytes() + uvs.GetOffsetBytes()
                          + (joints.IsEmpty() ? 0 : sizeof(YK_Vector4i)) + weights.GetOffsetBytes();

    for (auto i : YK_CountTo(vertices.m_count))
    {
        YK_SizeT baseIndex = i * baseOffset;

        if (!vertices.IsEmpty())
        {
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &vertices.m_buffer[i], vertices.GetOffsetBytes());
            baseIndex += vertices.GetOffsetBytes();
        }
        if (!normals.IsEmpty())
        {
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &normals.m_buffer[i], normals.GetOffsetBytes());
            baseIndex += normals.GetOffsetBytes();
        }
        if (!uvs.IsEmpty())
        {
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &uvs.m_buffer[i], uvs.GetOffsetBytes());
            baseIndex += uvs.GetOffsetBytes();
        }
        if (!joints.IsEmpty())
        {
            // Manually upcast to a 32 bit integer for creating the buffer
            YK_Vector4i joint{ static_cast<YK_Int32>(joints.m_buffer[i].x),
                               static_cast<YK_Int32>(joints.m_buffer[i].y),
                               static_cast<YK_Int32>(joints.m_buffer[i].z),
                               static_cast<YK_Int32>(joints.m_buffer[i].w) };
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &joint, sizeof(YK_Vector4i));
            baseIndex += sizeof(YK_Vector4i);
        }
        if (!weights.IsEmpty())
        {
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &weights.m_buffer[i], weights.GetOffsetBytes());
            baseIndex += weights.GetOffsetBytes();
        }
    }

    // Extract index data and upcast to 32 bit integers
    CG_GLTF::DataView<YK_U16> const indexBuffer = gltfMesh.ViewData<YK_U16>(primitive.indices);
    std::vector<YK_U32> indices;
    indices.resize(indexBuffer.m_count);
    for (auto i : YK_CountTo(indexBuffer.m_count))
    {
        indices[i] = static_cast<YK_U32>(indexBuffer.m_buffer[i]);
    }

    return CG_MeshFactory::FromData(interleavedData.data(),
                                    static_cast<YK_U32>(interleavedData.size() / sizeof(float)),
                                    indices.data(),
                                    static_cast<YK_U32>(indices.size()),
                                    layout);
}