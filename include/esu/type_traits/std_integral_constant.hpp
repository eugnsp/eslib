#pragma once

#include <type_traits>

namespace esu
{
template<auto value>
using Std_integral_constant = std::integral_constant<decltype(value), value>;
}
