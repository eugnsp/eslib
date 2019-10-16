#pragma once
#include <esu/bitwise.hpp>

#include <cassert>
#include <climits>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace esu
{
template<typename Value_, typename Size_ = std::size_t>
class Fenwick_tree
{
public:
	using Value = Value_;
	using Size = Size_;

public:
	// Constructs the empty Fenwick tree
	Fenwick_tree() = default;

	// Constructs the Fenwick tree of the given size initialized with the zero values
	Fenwick_tree(Size size) : data_(size)
	{
		assert(!is_empty());
	}

	// Constructs the Fenwick tree with the contents of the given vector
	Fenwick_tree(std::vector<Value> data) : data_(std::move(data))
	{
		assert(!is_empty());
		construct_in_place();
	}

	// Constructs the Fenwick tree with the contents of the range [first, last)
	template<class Input_iterator1, class Input_iterator2>
	Fenwick_tree(Input_iterator1 first, Input_iterator2 last) : data_(first, last)
	{
		assert(!is_empty());
		construct_in_place();
	}

	// Constructs the Fenwick tree with the contents of the given initializer list
	Fenwick_tree(std::initializer_list<Value> list) : Fenwick_tree(list.begin(), list.end())
	{}

	//////////////////////////////////////////////////////////////////////
	/** Capacity */

	// Checks if the container has no elements
	bool is_empty() const
	{
		return data_.empty();
	}

	// Returns the number of elements in the container
	Size size() const
	{
		return static_cast<Size>(data_.size());
	}

	// Returns the number of elements that the container has currently allocated space for
	Size capacity() const
	{
		return static_cast<Size>(data_.capacity());
	}

	//////////////////////////////////////////////////////////////////////
	/** Element access */

	// Returns the element with the given index
	Value operator[](Size index) const
	{
		assert(index < size());

		auto value = data_[index];
		for (Size mask = 1; (index & mask) != 0; mask <<= 1)
			value -= data_[index ^ mask];

		return value;
	}

	// Stores all elements in the range [dest, dest + size())
	template<class Random_access_iterator>
	void get(Random_access_iterator dest)
	{
		if (is_empty())
			return;

		auto index = size();
		do
		{
			--index;
			dest[index] = data_[index];
			if (const auto p = parent(index); p < size())
				dest[p] -= dest[index];
		} while (index != 0);
	}

	// Returns the sum of elements in the closed range [first, last]
	Value sum(Size first, Size last) const
	{
		assert(first <= last && last < size());

		auto sum = data_[last];

		last = last_child(last);
		while (last > first)
		{
			sum += data_[--last];
			last = last_child(last);
		}

		while (first != last)
		{
			sum -= data_[--first];
			first = last_child(first);
		}

		return sum;
	}

	// Returns the prefix sum for the element with the given index,
	// i.e. the sum of elements in the closed range [0, index]
	Value sum(Size index) const
	{
		return sum(0, index);
	}

	// Returns the sum of all elements; the container should be non-empty
	Value sum() const
	{
		assert(!is_empty());
		return sum(size() - 1);
	}

	// Returns the smallest index such that the prefix sum is not less than the given value,
	// or the container's size if no such index exists; the container should be non-empty and all
	// elements should be non-negative, so that the sequence of all prefix sums is non-decreasing
	Size lower_bound(Value value) const
	{
		assert(!is_empty());

		Size index = 0;
		for (auto mask = msb_size_mask(); mask != 0; mask >>= 1)
			if (const auto k = mask + index - 1; k < size() && data_[k] < value)
			{
				value -= data_[k];
				index += mask;
			}

		return index;
	}

	// Returns the smallest index such that the prefix sum is greater than the given value,
	// or the container's size if no such index exists; the container should be non-empty and all
	// elements should be non-negative, so that the sequence of all prefix sums is non-decreasing
	Size upper_bound(Value value) const
	{
		assert(!is_empty());

		Size index = 0;
		for (auto mask = msb_size_mask(); mask != 0; mask >>= 1)
			if (const auto k = mask + index - 1; k < size() && !(value < data_[k]))
			{
				value -= data_[k];
				index += mask;
			}

		return index;
	}

	//////////////////////////////////////////////////////////////////////
	/** Modifiers */

	// Replaces the contents with the given number of copies of the zero value
	void reset(Size size)
	{
		assert(size > 0);
		data_.assign(size, Value{});
	}

	// Replaces the contents with the contents of the given vector
	void reset(std::vector<Value> data)
	{
		assert(!data.empty());
		data_ = std::move(data);
		construct_in_place();
	}

	// Replaces the contents with copies of values in the given range [first, last)
	template<class Input_iterator1, class Input_iterator2>
	void reset(Input_iterator1 first, Input_iterator2 last)
	{
		assert(first != last);
		data_.assign(first, last);
		construct_in_place();
	}

	// Adds the given value to the element with the given index
	void add(Size index, const Value& value)
	{
		assert(index < size());
		for (; index < size(); index = parent(index))
			data_[index] += value;
	}

	// Sets the value of the element with the given index
	void set(Size index, const Value& value)
	{
		add(index, value - (*this)[index]);
	}

	// Appends the given element value to the end of the container
	void push(const Value& value)
	{
		data_.push_back(value);

		const auto index = size() - 1;
		for (Size mask = 1; (index & mask) != 0; mask <<= 1)
			data_.back() += data_[index ^ mask];
	}

	// Removes the last element of the container
	void pop()
	{
		data_.pop_back();
	}

private:
	// Returns the minimum size of the Fenwick tree such that the underlying binary tree
	// is perfect, i.e. returns (2^k) if the Fenwick tree size is in the range (2^{k-1}, 2^k]
	Size msb_size_mask() const
	{
		const auto offset = CHAR_BIT * sizeof(Size) - n_leading_zero_bits(size() - 1);
		return static_cast<Size>(1) << offset;
	}

	// Returns the index of the deepest left-most child
	static constexpr Size last_child(Size index)
	{
		return index & (index + 1);
	}

	// Returns the index of the parent node that is stored in the tree
	static constexpr Size parent(Size index)
	{
		return index | (index + 1);
	}

	// Converts the array of values into the Fenwick tree in-place
	void construct_in_place()
	{
		for (Size index = 0; index < size(); ++index)
			if (const auto p = parent(index); p < size())
				data_[p] += data_[index];
	}

private:
	std::vector<Value> data_;
};

template<class Input_iterator1, class Input_iterator2>
Fenwick_tree(Input_iterator1, Input_iterator2) -> Fenwick_tree<
	typename std::iterator_traits<Input_iterator1>::value_type,
	std::make_unsigned_t<typename std::iterator_traits<Input_iterator1>::difference_type>>;
} // namespace esu
