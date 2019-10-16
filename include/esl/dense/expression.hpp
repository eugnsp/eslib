#pragma once
#include <esl/dense/type_traits.hpp>

#include <cstddef>

namespace esl
{
template<class Expr>
class Expression
{
public:
	using Value = Value_type<Expr>;

public:
	///////////////////////////////////////////////////////////////////////
	//> Extents

	// Returns the number of rows
	std::size_t rows() const
	{
		return self().rows();
	}

	// Returns the number of columns
	std::size_t cols() const
	{
		return self().cols();
	}

	// Returns the number of elements = rows() * cols()
	std::size_t size() const
	{
		return rows() * cols();
	}

	// Check if the expression has zero number of elements
	bool is_empty() const
	{
		return rows() == 0 || cols() == 0;
	}

	///////////////////////////////////////////////////////////////////////
	//> CRTP

	Expr& self()
	{
		return static_cast<Expr&>(*this);
	}

	const Expr& self() const
	{
		return static_cast<const Expr&>(*this);
	}
};
} // namespace esl
