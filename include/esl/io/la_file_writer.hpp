#pragma once
#include <cstddef>
#include <esl/dense.hpp>
#include <esl/io/la_file.hpp>
#include <esu/time.hpp>
#include <fstream>
#include <string>

namespace esl
{
class La_file_writer
{
public:
	explicit La_file_writer(const std::string& file_name)
	{
		file_.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		file_.open(file_name, std::ofstream::binary);

		write_header();
	}

	template<typename T, std::size_t rows, std::size_t cols>
	void write(const std::string& var_name, const Matrix<T, rows, cols>& matrix)
	{
		const auto l = static_cast<std::uint32_t>(var_name.length());
		write_raw(l);
		file_.write(var_name.c_str(), var_name.length());

		const auto r = static_cast<std::uint32_t>(matrix.rows());
		write_raw(r);

		const auto c = static_cast<std::uint32_t>(matrix.cols());
		write_raw(c);

		file_.write(reinterpret_cast<const char*>(matrix.data()), matrix.size() * sizeof(T));
	}

	void close()
	{
		file_.close();
	}

private:
	template<typename T>
	void write_raw(T x)
	{
		file_.write(reinterpret_cast<const char*>(&x), sizeof(T));
	}

	void write_header()
	{
		std::string text("LA file, created on: " + esu::date_time_string());
		text.resize(64, ' ');

		file_.write(text.c_str(), la_file_header_length);

		const std::uint32_t version = 1;
		write_raw(version);
	}

private:
	std::ofstream file_;
};
} // namespace esl
