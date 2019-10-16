#pragma once

namespace esf
{
class Seq_cell_assembler
{
public:
	void init()
	{}

	template<class System, class Fn>
	void assemble(const System& system, Fn&& fn)
	{
		for (const auto& cell : system.mesh().cells())
			fn(cell);
	}
};

} // namespace esf
