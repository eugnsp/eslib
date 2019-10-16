#pragma once
#include <cassert>
#include <iterator>
#include <type_traits>
#include <utility>

namespace esu
{
template<class Iterator_begin, class Iterator_end = Iterator_begin>
class Iterable
{
public:
	Iterable() = default;

	Iterable(Iterator_begin begin, Iterator_end end) :
		begin_(std::move(begin)), end_(std::move(end))
	{}

	template<class Container>
	Iterable(Container&& container) :
		begin_(std::begin(container)), end_(std::end(container))
	{}

	template<class Container>
	Iterable(const Container&& container) = delete;

	Iterator_begin& begin()
	{
		return begin_;
	}

	Iterator_begin begin() const
	{
		return begin_;
	}

	Iterator_end& end()
	{
		return end_;
	}

	Iterator_end end() const
	{
		return end_;
	}

	auto size() const
	{
		const auto size = std::distance<Iterator_begin>(begin_, end_);
		assert(size > 0);

		return static_cast<std::make_unsigned_t<decltype(size)>>(size);
	}

	bool empty() const
	{
		return begin_ == end_;
	}

private:
	Iterator_begin begin_;
	Iterator_end end_;
};

template<class Container>
Iterable(Container&& container) ->
	Iterable<decltype(std::begin(container)), decltype(std::end(container))>;
} // namespace esu
