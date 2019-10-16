#pragma once
#include <esf/mesh/iterator/iterator_base.hpp>
#include <esf/types.hpp>

#include <cassert>
#include <iterator>
#include <type_traits>

namespace esf
{
template<class Element_tag, class Mesh>
class Random_access_iterator : public internal::Iterator_base<Element_tag, Mesh>
{
private:
	using Base = internal::Iterator_base<Element_tag, Mesh>;

public:
	using typename Base::View;

	//////////////////////////////////////////////////////////////////////////
	/** STL member types */

	using iterator_category = std::random_access_iterator_tag;
	using value_type = View;
	using difference_type = std::make_signed_t<Index>;
	using pointer = View*;
	using reference = View&;

public:
	using Base::Base;

	//////////////////////////////////////////////////////////////////////////
	/** Input/forward iterator members */

	Random_access_iterator& operator++()
	{
		return (*this += 1);
	}

	Random_access_iterator operator++(int)
	{
		auto old_iter{*this};
		++(*this);
		return old_iter;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Bidirectional iterator members */

	Random_access_iterator& operator--()
	{
		return (*this -= 1);
	}

	Random_access_iterator operator--(int) const
	{
		auto old_iter{*this};
		--(*this);
		return old_iter;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Random access iterator members */

	Random_access_iterator& operator+=(difference_type n)
	{
		view_.set_index(*view_ + n);
		return *this;
	}

	Random_access_iterator operator+(difference_type n) const
	{
		auto new_iter{*this};
		return (new_iter += n);
	}

	Random_access_iterator& operator-=(difference_type n)
	{
		view_.set_index(*view_ - n);
		return *this;
	}

	Random_access_iterator operator-(difference_type n) const
	{
		auto new_iter{*this};
		return (new_iter -= n);
	}

protected:
	using Base::view_;
};

//////////////////////////////////////////////////////////////////////////
/** Random access iterator free functions */

template<class Element_tag, class Mesh>
bool operator<(const Random_access_iterator<Element_tag, Mesh>& it1,
	const Random_access_iterator<Element_tag, Mesh>& it2)
{
	assert(&it1.mesh() == &it2.mesh());
	return **it1 < **it2;
}

template<class Element_tag, class Mesh>
bool operator>(const Random_access_iterator<Element_tag, Mesh>& it1,
	const Random_access_iterator<Element_tag, Mesh>& it2)
{
	return it2 < it1;
}

template<class Element_tag, class Mesh>
bool operator<=(const Random_access_iterator<Element_tag, Mesh>& it1,
	const Random_access_iterator<Element_tag, Mesh>& it2)
{
	return !(it2 < it1);
}

template<class Element_tag, class Mesh>
bool operator>=(const Random_access_iterator<Element_tag, Mesh>& it1,
	const Random_access_iterator<Element_tag, Mesh>& it2)
{
	return it2 <= it1;
}

template<class Element_tag, class Mesh>
Random_access_iterator<Element_tag, Mesh> operator+(
	typename Random_access_iterator<Element_tag, Mesh>::difference_type n,
	const Random_access_iterator<Element_tag, Mesh>& it)
{
	return it + n;
}

template<class Element_tag, class Mesh>
auto operator-(const Random_access_iterator<Element_tag, Mesh>& it1,
	const Random_access_iterator<Element_tag, Mesh>& it2)
{
	using T = typename Random_access_iterator<Element_tag, Mesh>::difference_type;
	return static_cast<T>(**it1) - static_cast<T>(**it2);
}
} // namespace esf
