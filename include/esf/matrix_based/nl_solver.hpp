#pragma once
#include <esf/matrix_based/solver_base.hpp>

#include <esl/dense.hpp>

#include <cstddef>

namespace esf
{
template<class System, class Linear_solver>
class Matrix_based_nl_solver : public Matrix_based_solver_base<System, Linear_solver>
{
private:
	using Base = Matrix_based_solver_base<System, Linear_solver>;

public:
	using Base::Base;
	using Base::system;

	template<class... Args>
	void init(Args&&... args)
	{
		Base::init(std::forward<Args>(args)...);
		step_.resize(system_.n_free_dofs());
	}

	void solve()
	{
		before_solve();

		set_bnd_values();
		linear_solver_.analyze();

		double residual_norm_old = 0;
		const double armijo = 1e-4;
		double reduction_factor = 1;

		unsigned int n_iter = 0;
		while (true)
		{
			assemble();
			const double residual_norm = rhs_norm2();

			if (n_iter > 0 && residual_norm > residual_norm_old * (1 - armijo * reduction_factor))
			{
				reduction_factor /= 2;
				step_ /= 2;
				solution_.rows_view(0, step_.size()) -= step_;

				after_line_search_step(n_iter, residual_norm, reduction_factor);
			}
			else
			{
				reduction_factor = 1;
				residual_norm_old = residual_norm;

				linear_solver_.factorize_solve(rhs_, step_);
				solution_.rows_view(0, step_.size()) += step_;

				const auto step_norm = esl::norm2(step_);
				const auto solution_norm = esl::norm2(solution_);
				after_step(n_iter, residual_norm, step_norm, solution_norm);

				if (step_norm / solution_norm < 1e-12 || n_iter > 50)
					break;
			}

			++n_iter;
		}
	}

	std::size_t memory_size() const
	{
		return Base::memory_size() + step_.memory_size();
	}

protected:
	virtual double rhs_norm2()
	{
		return esl::norm2(rhs_);
	}

	virtual void assemble() = 0;

	virtual void before_solve()
	{}

	virtual void after_step(unsigned int /* n_iter */,
							double /* residual_norm */,
							double /* step_norm */,
							double /* solution_norm */)
	{}

	virtual void after_line_search_step(unsigned int /* n_iter */,
										double /* residual_norm */,
										double /* reduction factor */)
	{}

protected:
	using Base::matrix_;
	using Base::rhs_;
	using Base::solution_;

	using Base::linear_solver_;
	using Base::system_;

	using Base::set_bnd_values;

private:
	esl::Vector_xd step_;
};
} // namespace esf
