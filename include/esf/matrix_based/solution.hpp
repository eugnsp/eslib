#pragma once
#include <esf/matrix_based/solution_base.hpp>
#include <esf/matrix_based/solution_view.hpp>

#include <esl/dense.hpp>

#include <cassert>

namespace esf
{
template<class System, typename Value>
class Solution : public internal::Solution_base<System, esl::Vector_x<Value>>
{
private:
	using Vector = esl::Vector_x<Value>;
	using Base = internal::Solution_base<System, Vector>;

public:
	using Base::Base;

	Solution(const Solution_view<System, Value>& solution) : Base(solution.system(), solution.values())
	{
		assert(&system() == &solution.system());
	}

	Solution& operator=(const Solution<System, Value>& solution)
	{
		//assert(&system() == &solution.system());
		values_ = solution.values();
		return *this;
	}

	Solution& operator=(const Solution_view<System, Value>& solution)
	{
		//assert(&system() == &solution.system());
		values_ = solution.values();
		return *this;
	}

private:
	using Base::system;
	using Base::values_;
};
} // namespace esf
