#include "PCH/CG_PCH.h"
#include "CG_ShaderLoader.h"

#include "CG/Resource/Shader/CG_Shader.h"

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

// Quick and dirty explicit parser
// This would ideally be handled by a specific data deserializer
CG_Shader CG_ShaderLoader::Load(YK_FilePath const& p_shaderPath)
{
    std::stringstream shaderFile;
    YK_IFile::GetFileContents(p_shaderPath.CString(), shaderFile);

    std::string vertex;
    std::string fragment;

    std::string fileLine;
    while (std::getline(shaderFile, fileLine))
    {
        StripWhitespace(fileLine);

        YK_SizeT equalsPos = fileLine.find_first_of('=');

        std::string_view attribute(fileLine.begin(), fileLine.begin() + equalsPos);
        // Trim quotation marks - Assumes there are quotation marks
        std::string_view value(fileLine.begin() + equalsPos + 2, fileLine.end() - 1);

        if (attribute == "vertex")
        {
            vertex = value;
        }
        else if (attribute == "fragment")
        {
            fragment = value;
        }
    }

    YK_FilePath vertexPath(vertex);
    YK_FilePath fragmentPath(fragment);

    return CG_Shader(vertexPath.CString(), fragmentPath.CString());
}