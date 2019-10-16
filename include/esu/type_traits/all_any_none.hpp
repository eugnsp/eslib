#pragma once
#include <type_traits>

namespace esu
{
// Checks whether all bool values are true; true if the pack is empty
template<bool... values>
struct All : std::conjunction<std::bool_constant<values>...>
{};

// Checks whether any bool value is true; false if the pack is empty
template<bool... values>
struct Any : std::disjunction<std::bool_constant<values>...>
{};

// Checks whether no bool value is true; true if the pack is empty
template<bool... values>
struct None : std::conjunction<std::bool_constant<!values>...>
{};

template<bool... values>
inline constexpr bool all = All<values...>::value;

template<bool... values>
inline constexpr bool any = Any<values...>::value;

template<bool... values>
inline constexpr bool none = None<values...>::value;
} // namespace esu
