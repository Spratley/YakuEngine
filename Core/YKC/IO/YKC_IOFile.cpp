#include "PCH/YakuCore_PCH.h"
#include "YKC_IOFile.h"

void YK_IFile::GetFileContents(const char* p_path, std::stringstream& p_outDestination)
{
	YK_IFile file(p_path);
	file.GetContents(p_outDestination);
}

YK_IFile::YK_IFile(const char* p_path)
{
	Open(p_path);
}

YK_IFile::~YK_IFile()
{
	Close();
}

bool YK_IFile::Open(const char* p_path)
{
#if !YAKU_RETAIL
	if (m_inputFileStream.is_open())
	{
		// Assert file is closed
	}
#endif // !YAKU_RETAIL

	m_inputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		m_inputFileStream.open(p_path);
	}
	catch (std::ifstream::failure /*error*/)
	{
		// TODO: LOG FAILED OPEN
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
		catch (std::ifstream::failure /*error*/)
		{
			// TODO: Log failed to read
		}
	}

	return false;
}

YK_OFile::YK_OFile(const char* p_path)
{
	Open(p_path);
}

YK_OFile::~YK_OFile()
{
	Close();
}

bool YK_OFile::Open(const char* p_path)
{
#if !YAKU_RETAIL
	if (m_outputFileStream.is_open())
	{
		// Assert file is closed
	}
#endif // !YAKU_RETAIL

	m_outputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		m_outputFileStream.open(p_path);
	}
	catch (std::ifstream::failure /*error*/)
	{
		// TODO: LOG FAILED OPEN
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

void YK_OFile::Write(std::string const& p_line)
{
	m_outputFileStream << p_line;
}