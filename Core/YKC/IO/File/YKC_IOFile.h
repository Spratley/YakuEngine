#pragma once

#include <fstream>
#include <sstream>
#include <string>

class YK_IFile
{
public:
    static void GetFileContents(const char* p_path, std::stringstream& p_outDestination);

public:
    YK_IFile() = default;
    YK_IFile(const char* p_path);
    ~YK_IFile();

    bool Open(const char* p_path);
    void Close();

    bool GetContents(std::stringstream& p_outDestination) const;

private:
    std::ifstream m_inputFileStream;
};

class YK_OFile
{
public:
    YK_OFile() = default;
    YK_OFile(const char* p_path);
    ~YK_OFile();

    bool Open(const char* p_path);
    void Close();

    // TODO: Replace with custom string type
    void Write(std::string const& line);

private:
    std::ofstream m_outputFileStream;
};