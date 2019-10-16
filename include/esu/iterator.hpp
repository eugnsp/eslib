#pragma once
#include <esu/tuple.hpp>
#include <esu/iterator/iterable.hpp>
#include <esu/iterator/smart_ptr_iterator.hpp>
#include <esu/iterator/transform_iterator.hpp>
#include <iterator>
#include <type_traits>
#include <cassert>

namespace esu
{
template<class... Ts>
class Tuple_ref : public std::tuple<Ts...>
{
private:
	using Base = std::tuple<Ts...>;

public:
	using Base::Base;

	Tuple_ref& operator=(const Tuple_ref<std::decay_t<Ts>...>& other)
	{
		assign(other, std::make_index_sequence<sizeof...(Ts)>{});
		return *this;
	}

private:
	template<std::size_t... ii>
	void assign(const Tuple_ref<std::decay_t<Ts>...>& other, std::index_sequence<ii...>)
	{
		(void(std::get<ii>(*this) = std::get<ii>(other)), ...);
	}
};

// template<class... T1, class... T2>
// void swap(Tuple_ref<T1...> t1, Tuple_ref<T2...> t2)
// {
// 	std::swap(t1, t2);
// 	return;
// }
} // namespace esu

namespace esu
{
template<class... Iterators>
class Zip_iterator
{
	// private:
	// 	using Ref_tuple = std::tuple<std::reference_wrapper<typename
	// std::iterator_traits<Iterators>::value_type>...>;

public:
	using value_type = std::tuple<typename std::iterator_traits<Iterators>::value_type...>;
	using reference = std::tuple<typename std::iterator_traits<Iterators>::reference...>;
	using pointer = std::tuple<typename std::iterator_traits<Iterators>::pointer...>;
	using difference_type =
		std::common_type_t<typename std::iterator_traits<Iterators>::difference_type...>;
	using iterator_category =
		std::common_type_t<typename std::iterator_traits<Iterators>::iterator_category...>;

public:
	Zip_iterator(Iterators... its) : its_{its...}
	{}

	reference operator*() const
	{
		return esu::tuple_map([](auto it) -> decltype(auto) { return *it; }, its_);
	}

	bool operator<(const Zip_iterator& other) const
	{
		return its_ < other.its_;
	}

	bool operator==(const Zip_iterator& other) const
	{
		return std::get<0>(its_) == std::get<0>(other.its_);
	}

	bool operator!=(const Zip_iterator& other) const
	{
		return !(*this == other);
	}

	difference_type operator-(const Zip_iterator& other) const
	{
		return std::get<0>(its_) - std::get<0>(other.its_);
	}

	Zip_iterator& operator+=(difference_type offset)
	{
		tuple_for_each([offset](auto& x) { x += offset; }, its_);
		return *this;
	}

	Zip_iterator& operator-=(difference_type offset)
	{
		tuple_for_each([offset](auto& x) { x -= offset; }, its_);
		return *this;
	}

	Zip_iterator& operator--()
	{
		tuple_for_each([](auto& x) { --x; }, its_);
		return *this;
	}

	Zip_iterator& operator++()
	{
		tuple_for_each([](auto& x) { ++x; }, its_);
		return *this;
	}

	Zip_iterator operator++(int)
	{
		auto tmp{*this};
		++tmp;
		return tmp;
	}

	Zip_iterator operator--(int)
	{
		auto tmp{*this};
		--tmp;
		return tmp;
	}

	Zip_iterator operator+(difference_type offset)
	{
		Zip_iterator tmp{*this};
		return (tmp += offset);
	}

	Zip_iterator operator-(difference_type offset)
	{
		Zip_iterator tmp{*this};
		return (tmp -= offset);
	}

	auto& iterators() const
	{
		return its_;
	}

private:
	std::tuple<Iterators...> its_;
	//	Ref_tuple refs_;
};

} // namespace esu

namespace std
{
template<class... Iterators>
void iter_swap(esu::Zip_iterator<Iterators...> it1, esu::Zip_iterator<Iterators...> it2)
{
	esu::tuple_for_each(
		[](auto it1, auto it2) { std::iter_swap(it1, it2); }, it1.iterators(), it2.iterators());
}
} // namespace std
