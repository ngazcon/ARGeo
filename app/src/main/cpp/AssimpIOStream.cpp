#include "AssimpIOStream.h"

#include "EmbeddedResources.h"
#include "StringHelper.h"

#include <stdexcept>

namespace argeo
{
	AssimpIOStream::AssimpIOStream(const std::string& pFile, const std::string& pMode)
	{
		m_file = EmbeddedResources::resource_open(pFile, pMode);
		if (m_file == nullptr)
		{
			throw std::runtime_error("Cannot open the file.");
		}
	}

	AssimpIOStream::~AssimpIOStream()
	{
		::fclose(m_file);
	}

	std::size_t AssimpIOStream::Read(void* pvBuffer, std::size_t pSize, std::size_t pCount)
	{
		return ::fread(pvBuffer, pSize, pCount, m_file);
	}

	std::size_t AssimpIOStream::Write(const void* pvBuffer, std::size_t pSize, std::size_t pCount)
	{
		return ::fwrite(pvBuffer, pSize, pCount, m_file);
	}

	aiReturn AssimpIOStream::Seek(std::size_t pOffset, aiOrigin pOrigin)
	{
		return ::fseek(m_file, pOrigin, pOffset) == 0 ? AI_SUCCESS : AI_FAILURE;
	}

	std::size_t AssimpIOStream::Tell() const
	{
		return ::ftell(m_file);
	}

	std::size_t AssimpIOStream::FileSize() const
	{
		return EmbeddedResources::resource_size(m_file);
	}

	void AssimpIOStream::Flush()
	{
		::fflush(m_file);
	}
}
