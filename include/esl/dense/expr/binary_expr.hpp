#pragma once
#include <esl/dense/dense.hpp>
#include <esl/dense/type_traits.hpp>

#include <cstddef>
#include <utility>

namespace esl
{
template<class Expr1_, class Expr2_, template<class, class> class Fn_>
class Binary_expr : public Dense<Binary_expr<Expr1_, Expr2_, Fn_>, Rvalue>
{
public:
	using Expr1 = Expr1_;
	using Expr2 = Expr2_;
	using Fn = Fn_<Expr1, Expr2>;

public:
	template<class Expr1_f, class Expr2_f>
	Binary_expr(Expr1_f&& expr1, Expr2_f&& expr2) :
		expr1_(std::forward<Expr1_f>(expr1)), expr2_(std::forward<Expr2_f>(expr2))
	{
		Fn::check_sizes(expr1_, expr2_);
	}

	std::size_t rows() const
	{
		return Fn::rows(expr1_, expr2_);
	}

	std::size_t cols() const
	{
		return Fn::cols(expr1_, expr2_);
	}

	decltype(auto) operator()(std::size_t row, std::size_t col) const
	{
		return Fn::element(expr1_, expr2_, row, col);
	}

	const Expr1& expr1() const
	{
		return expr1_;
	}

	const Expr2& expr2() const
	{
		return expr2_;
	}

private:
	internal::Expr_storage_type<const Expr1> expr1_;
	internal::Expr_storage_type<const Expr2> expr2_;
};

///////////////////////////////////////////////////////////////////////
//> Type traits

template<class Expr1, class Expr2, template<class, class> class Fn>
struct Traits<Binary_expr<Expr1, Expr2, Fn>>
{
	using Value = typename Fn<Expr1, Expr2>::Value;
	using Layout = Common_layout_tag<Expr1, Expr2>;

	static constexpr std::size_t rows = Fn<Expr1, Expr2>::ct_rows;
	static constexpr std::size_t cols = Fn<Expr1, Expr2>::ct_cols;
};
} // namespace esl
