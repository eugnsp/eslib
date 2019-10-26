#pragma once
#include <esl/dense.hpp>

template<typename T>
struct Type_trait_is_dense
{
	auto name()
	{
		return "Type trait is_dense<Expr>";
	}

	void operator()()
	{
		static_assert(esl::is_dense<esl::Matrix<T, std::size_t{2}, std::size_t{2}>>);
		static_assert(esl::is_dense<esl::Matrix<T, std::size_t{2}, esl::dynamic>>);
		static_assert(esl::is_dense<esl::Matrix<T, esl::dynamic, 2>>);
		static_assert(esl::is_dense<esl::Matrix<T, esl::dynamic, esl::dynamic>>);
	}
};
