#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <cmath>

namespace argeo
{
	class StringHelper
	{
	public:

		static std::vector<std::string> splitpath(
			const std::string& str,
			const std::set<char> delimiters)
		{
			std::vector<std::string> result;

			char const* pch = str.c_str();
			char const* start = pch;
			for (; *pch; ++pch)
			{
				if (delimiters.find(*pch) != delimiters.end())
				{
					if (start != pch)
					{
						std::string str(start, pch);
						result.push_back(str);
					}
					else
					{
						result.push_back("");
					}
					start = pch + 1;
				}
			}
			result.push_back(start);

			return result;
		}

		static std::string dirname(const std::string& pathname)
		{
			std::size_t pos = pathname.find_last_of("\\/");
			return (std::string::npos == pos)
				? ""
				: pathname.substr(0, pos);
		}

		static std::vector<std::string> &split(
			const std::string &s,
			char delim,
			std::vector<std::string> &elems)
		{
			std::stringstream ss(s);
			std::string item;
			while (std::getline(ss, item, delim))
			{
				elems.push_back(item);
			}
			return elems;
		}


		static std::vector<std::string> split(
			const std::string &s,
			char delim)
		{
			std::vector<std::string> elems;
			split(s, delim, elems);
			return elems;
		}

		static bool replace(
			std::string& str,
			const std::string& from,
			const std::string& to)
		{
			std::size_t start_pos = str.find(from);
			if (start_pos == std::string::npos)
				return false;
			str.replace(start_pos, from.length(), to);
			return true;
		}
	};
}
