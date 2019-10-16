// #pragma once
// #include "../../base/matrix.hpp"
// #include <esl/sparse.hpp>

// #include <mkl_solvers_ee.h>
// #include <mkl_types.h>

// #include <cassert>
// #include <stdexcept>
// #include <string>
// #include <utility>

// namespace esl
// {
// template<class Sparse_matrix_>
// class Feast_solver2
// {
// public:
// 	using Sparse_matrix = Sparse_matrix_;

// private:
// 	using Value = typename Sparse_matrix::Value;
// 	using Vector = Vector_x<Value>;
// 	using Matrix = Matrix_x<Value>;
// 	using Symmetry_tag = typename Sparse_matrix_::Symmetry_tag;

// public:
// 	Feast_solver2()
// 	{
// 		::mkl_sparse_ee_init(pm_);
// 		pm_[2] = 2;	// subspace iteration technique based on FEAST algorithm
// 	}

// 	bool solve(Sparse_matrix& a, Sparse_matrix& b, Matrix& eigen_vectors,
// 		Vector& eigen_values, unsigned int& n_eigen_values)
// 	{
// 		char which = 'S';
// 		auto m0 = static_cast<MKL_INT>(eigen_values.size());

// 		Vector_x<double> relResErrors(m0);

// 		esl::Mkl_sparse_matrix_ext a1(a);
// 		esl::Mkl_sparse_matrix_ext b1(b);

// 		::matrix_descr descr;
// 		descr.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
// 		descr.mode = SPARSE_FILL_MODE_UPPER;
// 		descr.diag = SPARSE_DIAG_NON_UNIT;

// 		MKL_INT k;

// 		const auto r = ::mkl_sparse_d_gv(&which, pm_, a1.handle(), descr, b1.handle(), descr, m0,
// 			&k, eigen_values.data(), eigen_vectors.data(), relResErrors.data());

// 		n_eigen_values = k;

// 		return r == SPARSE_STATUS_SUCCESS;
// 	}

// private:
// 	MKL_INT pm_[128];
// };
// } // namespace esl
