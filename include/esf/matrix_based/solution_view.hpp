#pragma once
#include <esf/matrix_based/solution_base.hpp>

#include <esl/dense.hpp>

namespace esf
{
template<class System, typename Value>
class Solution_view : public internal::Solution_base<System, const esl::Vector_x<Value>&>
{
private:
	using Vector = esl::Vector_x<Value>;
	using Base = internal::Solution_base<System, const Vector&>;

public:
	using Base::Base;
};
} // namespace esf
