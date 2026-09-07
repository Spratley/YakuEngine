#pragma once

#include "YK/Math/YK_Hash.h"
#include "YK/Types/Math/YK_Integer.h"

#include <string>
#include <string_view>

struct YK_FilePath
{
public:
    YK_FilePath() = delete;
    YK_FilePath(std::string_view p_localPath)
    {
        constexpr std::string_view rootDataPath("J:/Harbourfront/Data/");
        m_fullPath = rootDataPath;
        m_fullPath.append(p_localPath);

        YK_SizeT dotOffset = m_fullPath.find_last_of('.');
        if (dotOffset != std::string::npos)
        {
            m_extension = std::string_view(m_fullPath.begin() + dotOffset + 1, m_fullPath.end());
        }
    }

    ~YK_FilePath() = default;

    char const* CString() const { return m_fullPath.c_str(); }
    YK_SizeT Length() const { return m_fullPath.size(); }

    std::string_view const& Extension() const { return m_extension; }

    bool operator==(YK_FilePath const& p_rhs) const { return m_fullPath == p_rhs.m_fullPath; }

private:
    std::string m_fullPath;
    std::string_view m_extension = "";
};

namespace std
{
    template <>
    struct hash<YK_FilePath>
    {
        YK_SizeT operator()(YK_FilePath const& p_filePath) const noexcept
        {
            return YK_Hash::FNV_1A_64(p_filePath.CString());
        }
    };
} // namespace std