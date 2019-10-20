#pragma once

namespace esf
{
template<class Solver>
class Matrix_asm_base
{
public:
	Matrix_asm_base(Solver& solver) : solver_(solver)
	{}

	virtual ~Matrix_asm_base() = default;

	virtual void assemble() = 0;

protected:
	Solver& solver_;
};
} // namespace esf
