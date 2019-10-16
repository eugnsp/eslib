#pragma once
#include <esl/dense/storage/allocator.hpp>

#include <esu/type_traits.hpp>

#include <cstddef>
#include <utility>

namespace esl::internal
{
// Dynamic storage class
template<typename Value, std::size_t alignment = get_alignment<Value>()>
class Dynamic_storage
{
private:
	using Alloc = Allocator<Value, alignment>;

public:
	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Constructors and destructors

	Dynamic_storage() = default;

	explicit Dynamic_storage(const std::size_t size) : size_(size)
	{
		data_ = Alloc::allocate(size);
	}

	~Dynamic_storage() noexcept
	{
		Alloc::deallocate(data_);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Extents

	// Returns the size of the storage
	std::size_t size() const
	{
		return size_;
	}

	// Resizes the storage; this function call be called
	// only if the element type is trivially relocatable
	void resize(const std::size_t new_size)
	{
		static_assert(esu::is_trivially_relocatable<Value>);

		data_ = Alloc::reallocate(data_, new_size);
		size_ = new_size;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Element access

	Value& operator[](const std::size_t index)
	{
		return data_[index];
	}

	const Value& operator[](const std::size_t index) const
	{
		return data_[index];
	}

	[[gnu::assume_aligned(alignment)]]
	Value* data() noexcept
	{
		return data_;
	}

	[[gnu::assume_aligned(alignment)]]
	const Value* data() const noexcept
	{
		return data_;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//> Modifiers

	void swap(Dynamic_storage& other) noexcept
	{
		std::swap(data_, other.data_);
		std::swap(size_, other.size_);
	}

private:
	Value* data_ = nullptr;
	std::size_t size_ = 0;
};
} // namespace esl::internal
