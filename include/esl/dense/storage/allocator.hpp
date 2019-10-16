#pragma once
#include <esu/type_traits.hpp>

#include <mkl_service.h>
#include <mkl_version.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <new>
#include <stdexcept>
#include <type_traits>

#if INTEL_MKL_VERSION < 2110301
	#error Unsupported version of MKL
#endif


namespace esl::internal
{
template<typename T>
constexpr std::size_t get_alignment()
{
	constexpr std::size_t min_arithmetic_alignment = 64;
	if constexpr (std::is_arithmetic_v<T> && alignof(T) <= min_arithmetic_alignment)
		return min_arithmetic_alignment;
	else
		return alignof(T);
}

template<typename T, std::size_t alignment>
class Allocator
{
public:
	[[nodiscard, gnu::malloc, gnu::assume_aligned(alignment)]]
	static T* allocate(const std::size_t size)
	{
		if (size == 0)
			return nullptr;
		if (size > max_size)
			throw std::bad_array_new_length();

		if (const auto ptr = ::mkl_malloc(size * sizeof(T), alignment); ptr != nullptr)
			return static_cast<T*>(ptr);

		throw std::bad_alloc{};
	}

	[[nodiscard, gnu::assume_aligned(alignment)]]
	static T* reallocate(T* const old_ptr, std::size_t new_size)
	{
		static_assert(esu::is_trivially_relocatable<T>);

		if (old_ptr == nullptr)
			return allocate(new_size);

		if (new_size == 0)
		{
			deallocate(old_ptr);
			return nullptr;
		}
		if (new_size > max_size)
			throw std::bad_array_new_length();

		if (const auto ptr = ::mkl_realloc(old_ptr, new_size * sizeof(T)); ptr != nullptr)
		{
			// mkl_realloc() in MKL 2011.3.1 and later should preserve buffer alignment
			assert(reinterpret_cast<std::uintptr_t>(ptr) % alignment == 0);

			return static_cast<T*>(ptr);
		}

		throw std::bad_alloc{};
	}

	static void deallocate(T* const ptr) noexcept
	{
		::mkl_free(ptr);
	}

private:
	static constexpr auto max_size = static_cast<std::size_t>(-1) / sizeof(T);
};
} // namespace esl::internal
