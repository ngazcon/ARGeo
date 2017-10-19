#include "EmbeddedResources.h"
#include "include/JNIHelper.h"
#include "Log.h"
#include "StringHelper.h"

#include <sys/stat.h>
#include <fstream>
#include <vector>

namespace argeo
{
	bool EmbeddedResources::resource_directory_exists(const std::string& file_path)
	{
		std::string external_data_path = JNIHelper::get_instance().get_external_data_path();
		return directory_exists(external_data_path + get_os_separator() + file_path);
	}

	bool EmbeddedResources::directory_exists(const std::string& file_path)
	{
		struct stat buffer;
		int32_t res = stat(file_path.c_str(), &buffer);
		return res == 0 &&
			   S_ISDIR(buffer.st_mode);
	}

	bool EmbeddedResources::file_exists(const std::string& file_path)
	{
		struct stat buffer;
		return stat(file_path.c_str(), &buffer) == 0;
	}

	void EmbeddedResources::create_resource_path(const std::string& path)
	{
		std::string external_data_path = JNIHelper::get_instance().get_external_data_path();
		create_path(external_data_path + get_os_separator() + path);
	}

	void EmbeddedResources::create_path(const std::string& path)
	{
		std::vector<std::string> split_path = StringHelper::splitpath(path, std::set<char> { EmbeddedResources::get_os_separator() });
		std::string dir;

		for (int i = 0; i < split_path.size() - 1; i++)
		{
			if (i > 0)
			{
				dir += EmbeddedResources::get_os_separator();
			}
			dir += split_path[i];

			if (!directory_exists(dir))
			{
				mkdir(dir.c_str(), S_IRWXU | S_IRWXG);
			}
		}
	}

	inline bool external_exists(
		const std::string& resource_path,
		const char& os_separator)
	{
		std::string external_data_path = JNIHelper::get_instance().get_external_data_path();
		std::string external_file_path = external_data_path + os_separator + resource_path;
		if (!EmbeddedResources::file_exists(external_file_path))
		{
			return false;
		}
		return true;
	}

	inline bool internal_exists(
		const std::string& resource_path,
		const char& os_separator)
	{
		AAsset* asset = AAssetManager_open(
			JNIHelper::get_instance().get_asset_manager(),
			resource_path.c_str(),
			AASSET_MODE_UNKNOWN);

		std::string internal_data_path = JNIHelper::get_instance().get_internal_data_path();
		std::string internal_file_path = internal_data_path + os_separator + resource_path;
		if (!asset && !EmbeddedResources::file_exists(internal_file_path))
		{
			return false;
		}

		return true;
	}

	inline bool extract_asset(const std::string& resource_path)
	{
		// Open file
		AAsset* asset = AAssetManager_open(
			JNIHelper::get_instance().get_asset_manager(),
			resource_path.c_str(),
			AASSET_MODE_UNKNOWN);

		std::vector<char> asset_content;

		if (asset != NULL)
		{
			// Find size
			off_t asset_size = AAsset_getLength(asset);

			// Prepare input buffer
			asset_content.resize(asset_size);

			// Store input buffer
			AAsset_read(asset, asset_content.data(), asset_size);

			// Close
			AAsset_close(asset);
			
			std::string apk_workspace_path = JNIHelper::get_instance().get_external_data_path();
			std::string file_path          = apk_workspace_path + EmbeddedResources::get_os_separator() + resource_path;

			// Create the path if not exists.
			if (!EmbeddedResources::directory_exists(StringHelper::dirname(file_path)))
			{
				EmbeddedResources::create_path(file_path);
			}

			// Prepare output buffer
			std::ofstream asset_extracted(
				file_path.c_str(),
				std::ios::out | std::ios::binary);

			if (!asset_extracted)
			{
				return false;
			}

			// Write output buffer into a file
			asset_extracted.write(asset_content.data(), asset_content.size());
			asset_extracted.close();
		}
		else
		{
			return false;
		}

		return true;
	}


	bool EmbeddedResources::resource_exists(const std::string& resource_path)
	{
		return internal_exists(resource_path, get_os_separator()) ||
			   external_exists(resource_path, get_os_separator());
	}

	char EmbeddedResources::get_os_separator()
	{
		return '/';
	}

	std::size_t EmbeddedResources::resource_size(const std::string& resource_path)
	{
		struct stat stat_buf;
		int rc = stat(resource_path.c_str(), &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
	}

	std::size_t EmbeddedResources::resource_size(FILE* file)
	{
		struct stat stat_buf;
		int fd = ::fileno(file);
		int rc = ::fstat(fd, &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
	}

	void EmbeddedResources::resource_close(FILE* file)
	{
		::fclose(file);
	}

	FILE* EmbeddedResources::resource_open(
		const std::string& resource_path, 
		const std::string& mode)
	{
		std::string file_path;

		if ( internal_exists(resource_path, get_os_separator()) &&
			!external_exists(resource_path, get_os_separator()))
		{
			extract_asset(resource_path);
		}

		
		std::string external_data_path = JNIHelper::get_instance().get_external_data_path();
		file_path = external_data_path + get_os_separator() + resource_path;

		return ::fopen(file_path.c_str(), mode.c_str());
	}
}