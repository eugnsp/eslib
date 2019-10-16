#pragma once
#include <esu/algorithm/find.hpp>

#include <functional>

namespace esu
{
// Returns true if there exists a circular permutation of the elements in the range
// [first1, last1) that makes that range equal to the range [first2, last2), where
// (last2) denotes (first2 + (last1 - first1)), returns true if the first range is empty
template<
	class Forward_iterator1,
	class Forward_iterator2,
	class Forward_iterator3,
	class Binary_predicate>
bool is_circ_permutation(
	Forward_iterator1 first1,
	Forward_iterator2 last1,
	Forward_iterator3 first2,
	Binary_predicate pred)
{
	const auto equal =
		find_if(first1, last1, [&pred, &first2](const auto& v) { return pred(v, *first2); });

	if (equal == last1)
		return (first1 == last1);

	auto first = equal;
	++first;
	++first2;

	// Check [equal, last1)
	for (; first != last1; ++first, ++first2)
		if (!pred(*first, *first2))
			return false;

	// Check [first1, equal)
	for (; first1 != equal; ++first1, ++first2)
		if (!pred(*first1, *first2))
			return false;

	return true;
}

template<class Forward_iterator1, class Forward_iterator2, class Forward_iterator3>
bool is_circ_permutation(
	Forward_iterator1 first1, Forward_iterator2 last1, Forward_iterator3 first2)
{
	return is_circ_permutation(first1, last1, first2, std::equal_to<>{});
}
} // namespace esu
