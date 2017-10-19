#include "AssimpIOSystem.h"

#include "EmbeddedResources.h"
#include "StringHelper.h"
#include "AssimpIOStream.h"

#include <stdexcept>

namespace argeo
{
	AssimpIOSystem::AssimpIOSystem()
	{ }

	AssimpIOSystem::~AssimpIOSystem()
	{ }

	bool AssimpIOSystem::Exists(const char* pFile) const
	{
		return EmbeddedResources::resource_exists(pFile);
	}

	char AssimpIOSystem::getOsSeparator() const
	{
		return EmbeddedResources::get_os_separator();
	}

	Assimp::IOStream* AssimpIOSystem::Open(
		const char* pFile,
		const char* pMode)
	{
		std::vector<std::string> path = StringHelper::splitpath(pFile, std::set<char>{ getOsSeparator() });
		std::string filename = path.back();
		std::string directory;

		std::string filepath;

		if (path.size() > 1)
		{
			for (int i = 0; i < path.size() - 1; i++)
			{
				directory += path[i];
				if (i != path.size() - 2)
				{
					directory += getOsSeparator();
				}
			}
			PushDirectory(directory);
		}
		else
		{
			PushDirectory(CurrentDirectory());
		}

		filepath = CurrentDirectory().length() > 0 ?
			CurrentDirectory() + getOsSeparator() + filename :
			filename;

		return new AssimpIOStream(
			filepath,
			pMode);
	}

	void AssimpIOSystem::Close(Assimp::IOStream* pFile)
	{
		PopDirectory();
		delete pFile;
	}
}