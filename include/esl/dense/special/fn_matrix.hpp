#pragma once
#include <esl/dense/dense.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl
{
template<class Fn, class Layout = No_layout>
class Fn_matrix : public Dense<Fn_matrix<Fn, Layout>, Rvalue>
{
public:
	using Value = Value_type<Fn_matrix>;

public:
	Fn_matrix(std::size_t rows, std::size_t cols, Fn fn) : rows_(rows), cols_(cols), fn_(std::move(fn))
	{}

	std::size_t rows() const
	{
		return rows_;
	}

	std::size_t cols() const
	{
		return cols_;
	}

	Value operator()(std::size_t row, std::size_t col) const
	{
		return fn_(row, col);
	}

private:
	const std::size_t rows_;
	const std::size_t cols_;

	const Fn fn_;
};

///////////////////////////////////////////////////////////////////////
//> Type traits

template<class Fn, class Layout_>
struct Traits<Fn_matrix<Fn, Layout_>>
{
	using Value = std::invoke_result_t<Fn, std::size_t, std::size_t>;
	using Layout = Layout_;

	static constexpr std::size_t rows = dynamic;
	static constexpr std::size_t cols = dynamic;
};
} // namespace esl
