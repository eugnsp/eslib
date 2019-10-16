#pragma once
#include <esl/dense/type_traits.hpp>
#include <esl/sparse/mkl_sparse_matrix.hpp>

#include <type_traits>
#include <utility>

namespace esl
{
template<class Matrix>
class Mkl_sparse_matrix_ext : public Mkl_sparse_matrix<std::remove_reference_t<Matrix>>
{
private:
	using Base = Mkl_sparse_matrix<std::remove_reference_t<Matrix>>;

public:
	template<class Matrix_f>
	Mkl_sparse_matrix_ext(Matrix_f&& matrix) : matrix_(std::forward<Matrix_f>(matrix))
	{
		ESL_CALL_MKL_SPARSE(internal::mkl_sparse_create, handle_, matrix_);
	}

private:
	using Base::handle_;
	Matrix matrix_;
};

template<class Matrix>
Mkl_sparse_matrix_ext(Matrix &&)->Mkl_sparse_matrix_ext<Matrix>;
} // namespace esl
