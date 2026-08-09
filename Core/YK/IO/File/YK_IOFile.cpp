#include "PCH/YakuCore_PCH.h"
#include "YK_IOFile.h"

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/Logging/YK_Logger.h"

#include <fstream>
#include <sstream>
#include <string>

void YK_IFile::GetFileContents(const char* p_path, std::stringstream& p_outDestination)
{
    YK_IFile file(p_path);
    file.GetContents(p_outDestination);
}

YK_IFile::YK_IFile(const char* p_path) { Open(p_path); }

YK_IFile::~YK_IFile() { Close(); }

bool YK_IFile::Open(const char* p_path)
{
    YK_ASSERT(!m_inputFileStream.is_open(), "Attempting to open a file that's already open!");

    m_inputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        m_inputFileStream.open(p_path);
    }
    catch (std::ifstream::failure)
    {
        YK_LOG_ERROR_PARAM("Failed to open file: {}", p_path);
        return false;
    }
    return true;
}

void YK_IFile::Close()
{
    if (m_inputFileStream.is_open())
    {
        m_inputFileStream.close();
    }
}

bool YK_IFile::GetContents(std::stringstream& p_destination) const
{
    if (m_inputFileStream.is_open())
    {
        try
        {
            p_destination << m_inputFileStream.rdbuf();
            return true;
        }
        catch (std::ifstream::failure)
        {
            YK_LOG_ERROR("Failed to read file contents!");
        }
    }

    return false;
}

YK_OFile::YK_OFile(const char* p_path) { Open(p_path); }

YK_OFile::~YK_OFile() { Close(); }

bool YK_OFile::Open(const char* p_path)
{
    YK_ASSERT(!m_outputFileStream.is_open(), "Attempting to open a file that's already open!");

    m_outputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        m_outputFileStream.open(p_path);
    }
    catch (std::ifstream::failure)
    {
        YK_LOG_ERROR_PARAM("Failed to open file: {}", p_path);
        return false;
    }
    return true;
}

void YK_OFile::Close()
{
    if (m_outputFileStream.is_open())
    {
        m_outputFileStream.close();
    }
}

void YK_OFile::Write(std::string const& p_line) { m_outputFileStream << p_line; }