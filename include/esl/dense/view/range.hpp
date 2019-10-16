#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/type_traits.hpp>

#include <cassert>
#include <cstddef>

namespace esl
{
namespace internal
{
template<std::size_t ct_start, std::size_t ct_size>
class Range
{
	static_assert(ct_size > 0);

public:
	Range() = default;

	Range(std::size_t start, std::size_t size) noexcept
	{
		assert(start == ct_start && size == ct_size);
	}

	static constexpr std::size_t start()
	{
		return ct_start;
	}

	static constexpr std::size_t size()
	{
		return ct_size;
	}

	constexpr std::size_t operator[](std::size_t index) const
	{
		assert(index < ct_size);
		return ct_start + index;
	}

	static constexpr bool are_all_less(std::size_t max)
	{
		return ct_start + ct_size <= max;
	}
};

template<std::size_t ct_start>
class Range<ct_start, dynamic>
{
public:
	explicit Range(std::size_t size) noexcept : size_(size)
	{
		assert(size_ > 0);
	}

	Range(std::size_t start, std::size_t size) noexcept : Range(size)
	{
		assert(start == ct_start);
	}

	static constexpr std::size_t start()
	{
		return ct_start;
	}

	std::size_t size() const
	{
		return size_;
	}

	std::size_t operator[](std::size_t index) const
	{
		assert(index < size_);
		return ct_start + index;
	}

	bool are_all_less(std::size_t max) const
	{
		return ct_start + size_ <= max;
	}

private:
	const std::size_t size_;
};

template<std::size_t ct_size>
class Range<dynamic, ct_size>
{
	static_assert(ct_size > 0);

public:
	explicit Range(std::size_t start) noexcept : start_(start)
	{}

	Range(std::size_t start, std::size_t size) noexcept : Range(start)
	{
		assert(size == ct_size);
	}

	std::size_t start() const
	{
		return start_;
	}

	static constexpr std::size_t size()
	{
		return ct_size;
	}

	std::size_t operator[](std::size_t index) const
	{
		assert(index < ct_size);
		return start_ + index;
	}

	bool are_all_less(std::size_t max) const
	{
		return start_ + ct_size <= max;
	}

private:
	const std::size_t start_;
};

template<>
class Range<dynamic, dynamic>
{
public:
	Range(std::size_t start, std::size_t size) noexcept : start_(start), size_(size)
	{
		assert(size_ > 0);
	}

	std::size_t start() const
	{
		return start_;
	}

	std::size_t size() const
	{
		return size_;
	}

	std::size_t operator[](std::size_t index) const
	{
		return start_ + index;
	}

	bool are_all_less(std::size_t max) const
	{
		return start_ + size_ <= max;
	}

private:
	const std::size_t start_;
	const std::size_t size_;
};
} // namespace internal

///////////////////////////////////////////////////////////////////////
//> Type traits

template<std::size_t begin, std::size_t size_>
struct Traits<internal::Range<begin, size_>>
{
	static constexpr std::size_t size = size_;
};
} // namespace esl
