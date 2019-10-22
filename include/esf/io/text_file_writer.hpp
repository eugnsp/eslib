#pragma once
#include <esl/dense.hpp>

#include <cstddef>
#include <fstream>
#include <string>
#include <type_traits>

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

	template<typename... Ts>
	void write(const Ts&... values)
	{
		(do_write(values), ...);
	}

	template<typename... Ts>
	void write_ln(const Ts&... values)
	{
		write(values..., '\n');
	}

private:
	template<typename T,
			 typename = std::enable_if_t<std::is_scalar_v<T>>>
	void do_write(T value)
	{
		file_ << value;
	}

	template<class Expr,
			 class Value_category,
			 typename = std::enable_if_t<esl::is_vector_expr<Expr>>>
	void do_write(const esl::Dense<Expr, Value_category>& expr)
	{
		file_ << expr[0];
		for (std::size_t i = 1; i < expr.size(); ++i)
			file_ << '\t' << expr[i];
	}

private:
	std::ofstream file_;
};
} // namespace esf::internal
