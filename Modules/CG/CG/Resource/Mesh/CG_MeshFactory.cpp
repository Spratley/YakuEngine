#include "PCH/CG_PCH.h"
#include "CG_MeshFactory.h"

#include "YK/IO/File/YK_IOFile.h"

#include <charconv> // For extracting data from strings
#include <map> // I don't want this here...
#include <vector>

// TODO: This needs SUPER improvement. It's not good as it stands
CG_Mesh* CG_MeshFactory::LoadOBJ(char const* p_path)
{
    std::stringstream objFileContents;
    YK_IFile::GetFileContents(p_path, objFileContents);

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

    return FromData(vertices.data(),
                    static_cast<YK_U32>(vertices.size()),
                    indices.data(),
                    static_cast<YK_U32>(indices.size()));
}

CG_Mesh* CG_MeshFactory::FromData(float* p_vertexData,
                                  YK_U32 p_vertexDataCount,
                                  YK_U32* p_indexData,
                                  YK_U32 p_indexCount)
{
    // TODO: Setup factory policy
    CG_Mesh* mesh = new CG_Mesh(CG_GPUDataPolicy::FREE_RAM_DO_GPU_UPLOAD);
    mesh->SetData(p_vertexData, p_vertexDataCount, p_indexData, p_indexCount);
    return mesh;
}

CG_Mesh* CG_MeshFactory::Quad()
{
    // Packed positions and UVs (X, Y, Z, U, V), right now there's no way to NOT have UVs
    // TODO: Make FromData() and CG_Mesh configurable so vertex attributes can change
    float vertices[] = { 0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f };

    unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

    return FromData(vertices, 20, indices, 6);
}
