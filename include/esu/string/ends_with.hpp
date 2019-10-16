#pragma once
#include <functional>
#include <string_view>

namespace esu
{
// Checks if a string ends with a given substring
template<class Binary_predicate>
bool ends_with(std::string_view str, std::string_view ending, Binary_predicate pred)
{
	return ending.length() <= str.length() &&
		   std::equal(ending.rbegin(), ending.rend(), str.rbegin(), pred);
}

inline bool ends_with(std::string_view str, std::string_view ending)
{
	return esu::ends_with(str, ending, std::equal_to<>{});
}
} // namespace esu
