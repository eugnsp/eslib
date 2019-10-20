#pragma once
#include <esf/matrix_based/solver_base.hpp>

#include <esl/dense.hpp>

#include <cstddef>
#include <memory>

namespace esf
{
template<class System_, class Linear_solver_>
class Matrix_based_solver : public Matrix_based_solver_base<System_, Linear_solver_>
{
private:
	using Base = Matrix_based_solver_base<System_, Linear_solver_>;

public:
	using Base::Base;

	void solve()
	{
		matrix_.zero();
		rhs_ = 0;

		before_solve();

		set_bnd_values();
		assemble();
		after_assemble();

		linear_solver_.analyze_factorize_solve(rhs_, solution_.rows_view(0, rhs_.size()));

		after_solve();
	}

protected:
	//irtual void set_bnd_values() = 0;

	virtual void before_solve()
	{}

	virtual void after_solve()
	{}

	virtual void after_assemble()
	{}

	virtual void assemble() = 0;

protected:
	using Base::linear_solver_;
	using Base::matrix_;
	using Base::rhs_;
	using Base::solution_;

	using Base::set_bnd_values;
};
} // namespace esf
