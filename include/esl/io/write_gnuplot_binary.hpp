#pragma once
#include <esl/dense/dense.hpp>

#include <cstddef>
#include <fstream>
#include <string>

namespace esl
{
template<typename T = float, class Expr, class Category, class Row_titles, class Col_titles>
void write_gnuplot_binary(
	const std::string& file_name, const Dense<Expr, Category>& expr, Row_titles row_titles, Col_titles col_titles)
{
	std::ofstream file;
	file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	file.open(file_name, std::ofstream::binary);

	const auto rows = static_cast<T>(expr.cols());
	file.write(reinterpret_cast<const char*>(&rows), sizeof(T));

	for (std::size_t col = 0; col < expr.cols(); ++col)
	{
		const auto f = static_cast<T>(col_titles(col));
		file.write(reinterpret_cast<const char*>(&f), sizeof(T));
	}

	for (std::size_t row = 0; row < expr.rows(); ++row)
	{
		const auto f = static_cast<T>(row_titles(row));
		file.write(reinterpret_cast<const char*>(&f), sizeof(T));

		for (std::size_t col = 0; col < expr.cols(); ++col)
		{
			const auto f = static_cast<T>(expr(row, col));
			file.write(reinterpret_cast<const char*>(&f), sizeof(T));
		}
	}
}

template<typename T = float, class Expr, class Category>
void write_gnuplot_binary(const std::string& file_name, const Dense<Expr, Category>& expr)
{
	const auto id = [](auto i) { return i; };
	write_gnuplot_binary(file_name, expr, id, id);
}
} // namespace esl
