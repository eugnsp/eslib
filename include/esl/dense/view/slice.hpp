#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace esl
{
namespace internal
{
template<std::size_t ct_size>
class Slice
{
public:
	explicit Slice(Vector<std::size_t, ct_size> indices) : indices_(std::move(indices))
	{
		assert(!indices_.is_empty());
	}

	std::size_t size() const
	{
		return indices_.size();
	}

	std::size_t operator[](std::size_t index) const
	{
		return indices_[index];
	}

	bool are_all_less(std::size_t max) const
	{
		for (std::size_t i = 0; i < indices_.size(); ++i)
			if (indices_[i] >= max)
				return false;
		return true;
	}

private:
	const Vector<std::size_t, ct_size> indices_;
};
} // namespace internal

///////////////////////////////////////////////////////////////////////
//> Type traits

template<std::size_t size_>
struct Traits<internal::Slice<size_>>
{
	static constexpr std::size_t size = size_;
};
} // namespace esl
