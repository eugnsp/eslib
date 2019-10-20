#pragma once
#include <cstddef>
#include <fstream>
#include <string>

namespace esf::internal
{
class Text_file_writer
{
public:
	Text_file_writer(const std::string file_name)
	{
		file_.exceptions(std::ofstream::badbit | std::ofstream::failbit);
		file_.open(file_name);
	}

	template<typename T, typename... Ts>
    void write(const T& value, const Ts&... values)
    {
		file_ << value;
		if constexpr (sizeof...(Ts) > 0)
			(file_ << ... << values);
    }

    template<typename... Ts>
    void write_ln(const Ts&... values)
    {
        write(values..., '\n');
    }

private:
	std::ofstream file_;
};
}
