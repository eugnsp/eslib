#pragma once
#include <iterator>

namespace esu
{
// Checks if the predicate returns true for all adjacent pairs of elements in the range,
// returns true if the predicate returns true for all pairs, false otherwise,
// returns true if the range contains less than two elements.
template<class Forward_iterator, class Unary_predicate>
bool all_of_pairs(Forward_iterator first, Forward_iterator last, Unary_predicate pred)
{
	if (first == last)
		return true;

	for (auto next = std::next(first); next != last; ++first, ++next)
		if (!pred(*first, *next))
			return false;

	return true;
}

// Checks if the predicate returns true for at least one adjacent pair of elements
// in the range, returns true if the predicate returns true for at least one pair,
// false otherwise, returns false if the range contains less than two elements.
template<class Forward_iterator, class Unary_predicate>
bool any_of_pairs(Forward_iterator first, Forward_iterator last, Unary_predicate pred)
{
	if (first == last)
		return false;

	for (auto next = std::next(first); next != last; ++first, ++next)
		if (pred(*first, *next))
			return true;

	return false;
}

// Checks if the predicate returns true for no adjacent pairs of elements
// in the range, returns true if the predicate returns true for no pairs,
// false otherwise, returns true if the range contains less than two elements.
template<class Forward_iterator, class Unary_predicate>
bool none_of_pairs(Forward_iterator first, Forward_iterator last, Unary_predicate pred)
{
	if (first == last)
		return true;

	for (auto next = std::next(first); next != last; ++first, ++next)
		if (pred(*first, *next))
			return false;

	return true;
}
} // namespace esu
