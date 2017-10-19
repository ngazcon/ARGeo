#pragma once

#include <string>
#include <stdio.h>

namespace argeo
{
	class EmbeddedResources
	{
		///
		/// A class for handling access to resources.
		///

	public:
		
		///
		/// Open a new file for a given resource path with a custom mode.
		/// @param {std::string} resource_path The path to the resource.
		/// @param {std::string} [mode = rb] The mode of access to the resource.
		/// @return {FILE*} The resource file.
		///
		static FILE* resource_open(
			const std::string& resource_path, 
			const std::string& mode = "rb");
		

		///
		/// Close a resource file.
		/// @param {FILE*} The resource file to close.
		///
		static void resource_close(FILE* file);

		///
		/// Get the O.S directory path separator.
		/// @return {char} The O.S separator.
		///
		static char get_os_separator();

		///
		/// Get the size in bytes of the resource.
		/// @param {FILE*} The resource file.
		/// @return {std::size_t} The size of the resource.
		///
		static std::size_t resource_size(FILE* file);
		///
		/// Get the size in bytes of the resource.
		/// @param {std::string} resource_path The path to the resource.
		/// @return {std::size_t} The size of the resource.
		///
		static std::size_t resource_size(const std::string& resource_path);

		///
		/// Tests for the existence of a file at the given path. 
		/// @param {std::string} resource_path The path to the resource.
		/// @result {bool} True if the resource exists.
		///
		static bool resource_exists(const std::string& resource_path);


		static bool resource_directory_exists(const std::string& file_path);
		static bool directory_exists(const std::string& file_path);


		static bool file_exists(const std::string& file_path);

		static void create_resource_path(const std::string& path);

		static void create_path(const std::string& path);
	};
}
