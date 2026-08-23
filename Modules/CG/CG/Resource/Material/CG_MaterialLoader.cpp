#include "PCH/CG_PCH.h"
#include "CG_MaterialLoader.h"

#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Texture/CG_Texture.h"

#include "YK/Core/YK_Core.h"
#include "YK/IO/File/YK_IOFile.h"

#include <cctype>
#include <sstream>
#include <string>
#include <string_view>

namespace
{
    static void StripWhitespace(std::string& p_string)
    {
        bool inQuotes = false;
        YK_SizeT writeIndex = 0;

        for (YK_SizeT readIndex = 0; readIndex < p_string.size(); ++readIndex)
        {
            char c = p_string[readIndex];

            if (c == '"')
            {
                inQuotes = !inQuotes;
            }

            if (!inQuotes && std::isspace(c))
            {
                continue;
            }

            p_string[writeIndex++] = c;
        }

        YK_ASSERT(!inQuotes, "Unclosed quotation found!");
        p_string.resize(writeIndex);
    }
} // namespace

// This is a very temporary loader while material files have zero complexity
CG_Material CG_MaterialLoader::Load(YK_FilePath const& p_materialPath)
{
    std::stringstream shaderFile;
    YK_IFile::GetFileContents(p_materialPath.CString(), shaderFile);

    std::string shader;
    std::string texture;

    std::string fileLine;
    while (std::getline(shaderFile, fileLine))
    {
        StripWhitespace(fileLine);

        YK_SizeT equalsPos = fileLine.find_first_of('=');

        std::string_view attribute(fileLine.begin(), fileLine.begin() + equalsPos);
        // Trim quotation marks - Assumes there are quotation marks
        std::string_view value(fileLine.begin() + equalsPos + 2, fileLine.end() - 1);

        if (attribute == "shader")
        {
            shader = value;
        }
        else if (attribute == "texture")
        {
            texture = value;
        }
    }

    YK_Core& engine = YK_Core::GetEngine();
    CG_Shader const& shaderAsset = engine.GetAssetManager().GetAsset<CG_Shader>(YK_FilePath(shader));
    CG_Texture const& textureAsset = engine.GetAssetManager().GetAsset<CG_Texture>(YK_FilePath(texture));

    return CG_Material{ .m_shader = &shaderAsset, .m_texture = &textureAsset };
}