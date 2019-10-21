#pragma once
#include <esf/index.hpp>

namespace esf
{
template<class Quadr_>
struct Quadr_point_index
{
	using Quadr = Quadr_;
	Index index;
};
} // namespace esf
