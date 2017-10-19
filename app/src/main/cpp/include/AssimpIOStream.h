#pragma once

#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>

namespace argeo
{
	class AssimpIOStream : public Assimp::IOStream
	{
		friend class AssimpIOSystem;

	protected:
		// Constructor protected for private usage by AssimpIOSystem
		AssimpIOStream(
			const std::string& pFile,
			const std::string& pMode);

	public:
		~AssimpIOStream();

		std::size_t Read(
			void* pvBuffer,
			std::size_t pSize,
			std::size_t pCount);

		std::size_t Write(
			const void* pvBuffer,
			std::size_t pSize,
			std::size_t pCount);

		aiReturn Seek(
			std::size_t pOffset,
			aiOrigin pOrigin);

		std::size_t Tell() const;

		std::size_t FileSize() const;

		void Flush();

	protected:
		FILE* m_file;
	};
}