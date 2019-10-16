#pragma once
#include <esl/dense/dense.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>

#include <cstddef>
#include <limits>
#include <random>
#include <utility>

namespace esl
{
template<class Random_distribution, class Random_generator>
class Random_matrix : public Dense<Random_matrix<Random_distribution, Random_generator>, Rvalue>
{
public:
	using Value = Value_type<Random_matrix>;

public:
	Random_matrix(const std::size_t rows, const std::size_t cols, Random_distribution distr, Random_generator& gen) :
		rows_(rows), cols_(cols), distr_(std::move(distr)), gen_(gen)
	{}

	std::size_t rows() const
	{
		return rows_;
	}

	std::size_t cols() const
	{
		return cols_;
	}

	Value operator()(const std::size_t, const std::size_t) const
	{
		return distr_(gen_);
	}

private:
	const std::size_t rows_;
	const std::size_t cols_;

	mutable Random_distribution distr_;
	Random_generator& gen_;
};

template<typename T = double>
Matrix_x<T> random_real_matrix(std::size_t rows, std::size_t cols, const T a = 0, const T b = 1)
{
	std::random_device rd;
	std::mt19937 gen{rd()};

	return Random_matrix(rows, cols, std::uniform_real_distribution<T>(a, b), gen);
}

template<typename T = double>
Matrix_x<T> random_real_matrix(std::size_t size)
{
	return random_real_matrix<T>(size, size);
}

template<typename T = int>
Matrix_x<T> random_int_matrix(
	std::size_t rows, std::size_t cols, const T a = 0, const T b = std::numeric_limits<T>::max())
{
	std::random_device rd;
	std::mt19937 gen{rd()};

	return Random_matrix(rows, cols, std::uniform_int_distribution<T>(a, b), gen);
}

template<typename T = int>
Matrix_x<T> random_int_matrix(std::size_t size)
{
	return random_int_matrix<T>(size, size);
}
///////////////////////////////////////////////////////////////////////
//> Type traits

template<class Random_distribution, class Random_generator>
struct Traits<Random_matrix<Random_distribution, Random_generator>>
{
	using Value = typename Random_distribution::result_type;
	using Layout = No_layout;

	static constexpr std::size_t rows = dynamic;
	static constexpr std::size_t cols = dynamic;
};
} // namespace esl
