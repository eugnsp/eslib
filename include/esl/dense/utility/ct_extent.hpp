#pragma once
#include <esl/dense/tags.hpp>

#include <cstddef>

namespace esl::internal
{
constexpr bool is_extent_dynamic_or_eq(std::size_t ext1, std::size_t ext2)
{
	return ext1 == dynamic || ext2 == dynamic || ext1 == ext2;
}

constexpr bool is_extent_dynamic_or_less(std::size_t ext1, std::size_t ext2)
{
	return ext1 == dynamic || ext2 == dynamic || ext1 < ext2;
}

constexpr bool is_extent_dynamic_or_less_equal(std::size_t ext1, std::size_t ext2)
{
	return ext1 == dynamic || ext2 == dynamic || ext1 <= ext2;
}

constexpr bool is_extent_static_and_eq(std::size_t ext1, std::size_t ext2)
{
	return ext1 != dynamic && ext2 != dynamic && ext1 == ext2;
}

///////////////////////////////////////////////////////////////////////

constexpr std::size_t extent_static_or_zero(std::size_t ext)
{
	return (ext == dynamic) ? 0 : ext;
}

constexpr auto extent_static(std::size_t ext1, std::size_t ext2)
{
	return (ext1 == dynamic) ? ext2 : ext1;
}

constexpr auto extent_mul(std::size_t ext1, std::size_t ext2)
{
	return (ext1 == dynamic || ext2 == dynamic) ? dynamic : ext1 * ext2;
}
} // namespace esl::internal
