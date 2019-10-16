#pragma once
#include <ctime>
#include <string>

namespace esu
{
inline std::string date_time_string(const std::string& format = "%a, %d %b %Y %T")
{
	auto tm = std::time(nullptr);
	std::tm ltm;

#ifdef __unix__
	localtime_r(&tm, &ltm);
#else
	localtime_s(&ltm, &tm);
#endif

	char time[100];
	std::strftime(time, sizeof(time), format.c_str(), &ltm);
	return std::string(time);
}
} // namespace esu
