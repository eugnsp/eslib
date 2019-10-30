#pragma once
#include <cstddef>

namespace esf
{
template<class Quadr_>
struct Quadr_point_index final
{
	using Quadr = Quadr_;
	std::size_t index;
};
} // namespace esf
