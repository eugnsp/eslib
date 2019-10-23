#pragma once
#include <esl/dense/tags.hpp>

#include <cstddef>

namespace esl::internal
{
inline constexpr bool is_extent_dynamic_or_eq(const std::size_t ext1, const std::size_t ext2)
{
	return ext1 == dynamic || ext2 == dynamic || ext1 == ext2;
}

inline constexpr bool is_extent_dynamic_or_less(const std::size_t ext1, const std::size_t ext2)
{
	return ext1 == dynamic || ext2 == dynamic || ext1 < ext2;
}

inline constexpr bool is_extent_dynamic_or_less_equal(
	const std::size_t ext1, const std::size_t ext2)
{
	return ext1 == dynamic || ext2 == dynamic || ext1 <= ext2;
}

inline constexpr bool is_extent_static_and_eq(const std::size_t ext1, const std::size_t ext2)
{
	return ext1 != dynamic && ext2 != dynamic && ext1 == ext2;
}

///////////////////////////////////////////////////////////////////////

inline constexpr std::size_t extent_static_or_zero(const std::size_t ext)
{
	return (ext == dynamic) ? 0 : ext;
}

inline constexpr std::size_t extent_static(const std::size_t ext1, const std::size_t ext2)
{
	return (ext1 == dynamic) ? ext2 : ext1;
}

inline constexpr std::size_t extent_mul(const std::size_t ext1, const std::size_t ext2)
{
	return (ext1 == dynamic || ext2 == dynamic) ? dynamic : ext1 * ext2;
}
} // namespace esl::internal
