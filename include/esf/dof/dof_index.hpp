#pragma once
#include <esf/types.hpp>

namespace esf
{
struct Dof_index
{
public:
	Index index = invalid_index;
	bool is_free = true;

	bool is_valid() const
	{
		return index != invalid_index;
	}

	Dof_index& operator+=(Local_index delta)
	{
		index += delta;
		return *this;
	}

	Dof_index& operator-=(Local_index delta)
	{
		index -= delta;
		return *this;
	}
};

inline Dof_index operator+(Dof_index index, Local_index delta)
{
	return (index += delta);
}

inline Dof_index operator+(Local_index delta, Dof_index index)
{
	return index + delta;
}

inline Dof_index operator-(Dof_index index, Local_index delta)
{
	return (index -= delta);
}
} // namespace esf
