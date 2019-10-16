#pragma once
#include "../types.hpp"
#include "element_base.hpp"
#include <es/math/function.hpp>
#include <esl/dense.hpp>
//#include "math/la/Vector.h"
//#include "Util/Array.h"
#include <array>
#include <cstddef>
#include <esu/type_traits.hpp>
#include <utility>

namespace esf
{
class Edge_el : public internal::Triangular_element<0, 1, 0>
{};

class Exp_df_el : public internal::Triangular_element<1, 0, 0>
{
public:
};

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/
} // namespace esf
