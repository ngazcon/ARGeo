#pragma once

#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>

namespace argeo
{
	class AssimpIOSystem : public Assimp::IOSystem
	{
	public:
		AssimpIOSystem();

		~AssimpIOSystem();

		// Check whether a specific file exists
		bool Exists(const char* pFile) const;

		// Get the path delimiter character we'd like to see
		char getOsSeparator() const;

		// ... and finally a method to open a custom stream
		Assimp::IOStream* Open(
			const char* pFile,
			const char* pMode);

		void Close(Assimp::IOStream* pFile);

	};
}