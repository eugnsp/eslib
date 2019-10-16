#pragma once
#include <esl/dense/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <ostream>

namespace esl
{
template<class Expr, bool print_info>
class Printer
{
public:
	explicit Printer(const Expr& expr, const int width) : expr_(expr), width_(width)
	{
		assert(width >= 0);
	}

	void operator()(std::ostream& os) const
	{
		if constexpr (print_info)
		{
			os << expr_.rows() << " x " << expr_.cols();
			if constexpr (is_row_major<Expr>)
				os << " row-major";
			if constexpr (is_col_major<Expr>)
				os << " column-major";
			if constexpr (is_lvalue_block<Expr>)
				os << ", row stride = " << expr_.row_stride() << ", col_stride = " << expr_.col_stride()
				   << ", lead_dim = " << expr_.lead_dim();
			os << '\n';
		}

		for (std::size_t row = 0; row < expr_.rows(); ++row)
		{
			for (std::size_t col = 0; col < expr_.cols(); ++col)
			{
				if (col != 0)
					os << ' ';
				if (width_ != 0)
					os.width(width_);
				os << expr_(row, col);
			}
			os << '\n';
		}
	}

private:
	const Expr& expr_;
	const int width_;
};

template<class Expr>
auto printer(const Expr& expr, const int width = 0)
{
	return Printer<Expr, false>{expr, width};
}

template<class Expr>
auto printer_i(const Expr& expr, const int width = 0)
{
	return Printer<Expr, true>{expr, width};
}

template<class Expr, bool print_info>
std::ostream& operator<<(std::ostream& os, const Printer<Expr, print_info>& printer)
{
	printer(os);
	return os;
}
} // namespace esl
