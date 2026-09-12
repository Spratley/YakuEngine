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

template <typename T>
struct ModelDataView
{
    constexpr bool IsUsed() const { return m_count != 0; }
    constexpr YK_SizeT SizeBytes() const { return m_count * sizeof(T); }
    constexpr YK_SizeT GetTypeOffset() const { return IsUsed() ? sizeof(T) : 0; }

    T const* m_buffer = nullptr;
    YK_SizeT m_count = 0;
};

CG_Mesh CG_MeshLoader::LoadGLTF(YK_FilePath const& p_path)
{
    CG_GLTF gltfMesh(p_path);
    if (gltfMesh.CheckErrors() || !gltfMesh.HasMesh())
    {
        return CG_Mesh();
    }

    tg3_model const& model = gltfMesh.GetModel();
    tg3_primitive const& primitive = model.meshes[0].primitives[0];

    // Joints are stored as 8 bit integers in GLTF exported from Blender, so we need a vector to store them
    using YK_Vector4b = YK_Vector_N<YK_Byte, 4>;

    CG_MeshLayout layout;
    ModelDataView<YK_Vector3f> vertices;
    ModelDataView<YK_Vector3f> normals;
    ModelDataView<YK_Vector2f> uvs;
    ModelDataView<YK_Vector4b> joints;
    ModelDataView<YK_Vector4f> weights;

    for (auto i : YK_CountTo(primitive.attributes_count))
    {
        auto [name, value] = primitive.attributes[i];

        tg3_accessor const& accessor = model.accessors[value];
        tg3_buffer_view const& bufferView = model.buffer_views[accessor.buffer_view];
        tg3_buffer const& buffer = model.buffers[bufferView.buffer];

        YK_ASSERT(accessor.sparse.count == 0, "YakuEn doesn't support sparse GLTF data!");
        YK_ASSERT(bufferView.byte_stride == 0, "YakuEn doesn't support non-continuous GLTF data!");

        YK_U8 const* bufferStart = buffer.data.data + accessor.byte_offset + bufferView.byte_offset;

        if (std::strcmp(name.data, "POSITION") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::POSITION);
            vertices = { .m_buffer = reinterpret_cast<YK_Vector3f const*>(bufferStart), .m_count = accessor.count };
            YK_ASSERT(accessor.type == TG3_TYPE_VEC3, "Expected Vector3 data for positions!");
        }
        else if (std::strcmp(name.data, "NORMAL") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::NORMAL);
            normals = { .m_buffer = reinterpret_cast<YK_Vector3f const*>(bufferStart), .m_count = accessor.count };
            YK_ASSERT(accessor.type == TG3_TYPE_VEC3, "Expected Vector3 data for normals!");
        }
        else if (std::strcmp(name.data, "TEXCOORD_0") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::UV);
            uvs = { .m_buffer = reinterpret_cast<YK_Vector2f const*>(bufferStart), .m_count = accessor.count };
            YK_ASSERT(accessor.type == TG3_TYPE_VEC2, "Expected Vector2 data for UVs!");
        }
        else if (std::strcmp(name.data, "JOINTS_0") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::JOINT);
            joints = { .m_buffer = reinterpret_cast<YK_Vector4b const*>(bufferStart), .m_count = accessor.count };
            YK_ASSERT(accessor.type == TG3_TYPE_VEC4 && accessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_BYTE,
                      "Expected Vector4 of Byte data for Joints!");
        }
        else if (std::strcmp(name.data, "WEIGHTS_0") == 0)
        {
            layout.SetEnabled(CG_MeshAttribute::WEIGHT);
            weights = { .m_buffer = reinterpret_cast<YK_Vector4f const*>(bufferStart), .m_count = accessor.count };
            YK_ASSERT(accessor.type == TG3_TYPE_VEC4 && accessor.component_type == TG3_COMPONENT_TYPE_FLOAT,
                      "Expected Vector4 data for Weights!");
        }
    }

    // Joints needs 4x the contribution since we're increasing the size of each member to a full 32 bit integer
    YK_SizeT dataArrayByteCount =
      vertices.SizeBytes() + normals.SizeBytes() + uvs.SizeBytes() + (joints.SizeBytes() * 4) + weights.SizeBytes();

    std::vector<YK_Byte> interleavedData;
    interleavedData.resize(dataArrayByteCount);

    // Joints needs to manually offset by a Vector4
    YK_SizeT baseOffset = vertices.GetTypeOffset() + normals.GetTypeOffset() + uvs.GetTypeOffset()
                          + (joints.IsUsed() ? sizeof(YK_Vector4i) : 0) + weights.GetTypeOffset();

    for (auto i : YK_CountTo(vertices.m_count))
    {
        YK_SizeT baseIndex = i * baseOffset;

        if (vertices.IsUsed())
        {
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &vertices.m_buffer[i], vertices.GetTypeOffset());
            baseIndex += vertices.GetTypeOffset();
        }
        if (normals.IsUsed())
        {
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &normals.m_buffer[i], normals.GetTypeOffset());
            baseIndex += normals.GetTypeOffset();
        }
        if (uvs.IsUsed())
        {
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &uvs.m_buffer[i], uvs.GetTypeOffset());
            baseIndex += uvs.GetTypeOffset();
        }
        if (joints.IsUsed())
        {
            // Manually upcast to a 32 bit integer for creating the buffer
            YK_Vector4i joint{ static_cast<YK_Int32>(joints.m_buffer[i].x),
                               static_cast<YK_Int32>(joints.m_buffer[i].y),
                               static_cast<YK_Int32>(joints.m_buffer[i].z),
                               static_cast<YK_Int32>(joints.m_buffer[i].w) };
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &joint, sizeof(YK_Vector4i));
            baseIndex += sizeof(YK_Vector4i);
        }
        if (weights.IsUsed())
        {
            memcpy(static_cast<void*>(&interleavedData[baseIndex]), &weights.m_buffer[i], weights.GetTypeOffset());
            baseIndex += weights.GetTypeOffset();
        }
    }

    // Extract index data
    tg3_accessor const& accessor = model.accessors[primitive.indices];
    tg3_buffer_view const& bufferView = model.buffer_views[accessor.buffer_view];
    tg3_buffer const& indexBuffer = model.buffers[bufferView.buffer];

    YK_ASSERT(accessor.type == TG3_TYPE_SCALAR, "Expected scalar data for index array!");
    YK_ASSERT(accessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_SHORT, "Expected UInt16 type for index array!");

    YK_U16 const* indexBufferData =
      reinterpret_cast<YK_U16 const*>(indexBuffer.data.data + accessor.byte_offset + bufferView.byte_offset);
    std::vector<YK_U32> indices;
    indices.resize(accessor.count);
    for (auto i : YK_CountTo(accessor.count))
    {
        indices[i] = static_cast<YK_U32>(indexBufferData[i]);
    }

    return CG_MeshFactory::FromData(interleavedData.data(),
                                    static_cast<YK_U32>(interleavedData.size() / sizeof(float)),
                                    indices.data(),
                                    static_cast<YK_U32>(indices.size()),
                                    layout);
}