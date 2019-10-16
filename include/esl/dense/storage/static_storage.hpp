#pragma once
#include <esl/dense/storage/allocator.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl::internal
{
// Static storage class
template<typename Value, std::size_t ct_size, std::size_t alignment = get_alignment<Value>()>
class Static_storage
{
	static_assert(ct_size > 0);

public:
	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Constructors

	Static_storage() = default;

	template<typename... Values>
	explicit constexpr Static_storage(Values&&... values) : data_{std::forward<Values>(values)...}
	{
		static_assert(sizeof...(Values) == ct_size);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Extents

	static constexpr std::size_t size() noexcept
	{
		return ct_size;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Element access

	constexpr Value& operator[](const std::size_t index)
	{
		assert(index < size());
		return data_[index];
	}

	constexpr const Value& operator[](const std::size_t index) const
	{
		assert(index < size());
		return data_[index];
	}

	[[gnu::assume_aligned(alignment)]]
	constexpr Value* data() noexcept
	{
		return data_;
	}

	[[gnu::assume_aligned(alignment)]]
	constexpr const Value* data() const noexcept
	{
		return data_;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Modifiers

	constexpr void swap(Static_storage& other) noexcept(std::is_nothrow_swappable_v<Value>)
	{
		std::swap(data_, other.data_);
	}

private:
	alignas(alignment) Value data_[ct_size];
};

// Static zero-sized storage class
template<typename Value, std::size_t alignment>
class Static_storage<Value, 0, alignment>
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Extents

	static constexpr std::size_t size() noexcept
	{
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Element access

	constexpr Value& operator[](std::size_t)
	{
		return *static_cast<Value*>(nullptr);
	}

	constexpr const Value& operator[](std::size_t) const
	{
		return *static_cast<Value*>(nullptr);
	}

	constexpr Value* data() noexcept
	{
		return nullptr;
	}

	constexpr const Value* data() const noexcept
	{
		return nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Modifiers

	constexpr void swap(Static_storage&) noexcept
	{}
};
} // namespace esl::internal
