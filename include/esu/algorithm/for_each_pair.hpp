#pragma once
#include <iterator>

namespace esu
{
// Applies a given function object to adjacent pairs of elements
// in the range [first, last), in order.
//
// Parameters:
//  first, last - the range to apply the function to,
//  fn - the function object to be applied, the signature should be
//       equivalent to the following: void fn(T& a, T& b);
//       the type T must be such that an object of type Forward_iterator1
//       can be dereferenced and then implicitly converted to T.
//
// Return value:
//  the iterator pointing to the last element in the range,
//  first if the range contains less than two elements.
template<class Forward_iterator1, class Forward_iterator2, class Fn>
Forward_iterator1 for_each_pair(Forward_iterator1 first, Forward_iterator2 last, Fn&& fn)
{
	if (first == last)
		return first;

	auto next = std::next(first);
	while (next != last)
		fn(*first++, *next++);

	return first;
}

// Applies a given function object to adjacent pairs of elements
// in the range [first, first + n), in order.
//
// Parameters:
//  first - the beginning of the range to apply the function to,
//  n - the number of pairs to apply the function to
//  <func> - the function object to be applied, the signature should be
//           equivalent to the following: <void func(T& a, T& b);>.
//
// Return value:
//  the iterator pointing to the last element in the range,
//  <first> if the range contains less than two elements (n <= 0).
template<class Forward_iterator, typename N, class Fn>
Forward_iterator for_each_pair_n(Forward_iterator first, N n, Fn&& fn)
{
	auto next = first;
	for (N i = 0; i < n; ++i)
		fn(*first++, *++next);

	return first;
}
} // namespace esu
