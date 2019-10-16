#pragma once
#include <esu/type_traits.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace esu
{
// An iterator adapter with the same behavior as the underlying iterator except that
// its dereference operator returns an object managed by a smart pointer, and not
// smart pointer itself; if the underlying iterator is constant iterator, dereferencing
// produces a constant reference.
template<class Iterator>
class Smart_ptr_iter
{
private:
	using Traits = std::iterator_traits<Iterator>;
	static constexpr bool is_const_it =
		std::is_const_v<std::remove_reference_t<typename Traits::reference>>;

public:
	using iterator_category = typename Traits::iterator_category;
	using difference_type = typename Traits::difference_type;
	using value_type = typename Traits::value_type::element_type;
	using reference = std::add_lvalue_reference_t<Add_const_if<is_const_it, value_type>>;
	using pointer = Add_const_to_pointer_if<is_const_it, typename Traits::value_type::pointer>;

public:
	explicit Smart_ptr_iter(Iterator base = {}) : base_{std::move(base)}
	{}

	//////////////////////////////////////////////////////////////////////////

	// Returns the underlying iterator
	const Iterator& base() const
	{
		return base_;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Input/forward iterator members */

	reference operator*() const
	{
		return **base_;
	}

	pointer operator->() const
	{
		return base_->get();
	}

	Smart_ptr_iter& operator++()
	{
		++base_;
		return *this;
	}

	Smart_ptr_iter operator++(int) const
	{
		auto old_iter{*this};
		++base_;
		return old_iter;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Bidirectional iterator members */

	Smart_ptr_iter& operator--()
	{
		--base_;
		return *this;
	}

	Smart_ptr_iter operator--(int) const
	{
		auto old_iter{*this};
		--base_;
		return old_iter;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Random access iterator members */

	Smart_ptr_iter& operator+=(difference_type n)
	{
		base_ += n;
		return *this;
	}

	Smart_ptr_iter operator+(difference_type n) const
	{
		return Smart_ptr_iter(base_ + n);
	}

	Smart_ptr_iter& operator-=(difference_type n)
	{
		base_ -= n;
		return *this;
	}

	Smart_ptr_iter operator-(difference_type n) const
	{
		return Smart_ptr_iter(base_ - n);
	}

	reference operator[](difference_type n) const
	{
		return *base_[n];
	}

private:
	Iterator base_; // Underlying iterator
};

//////////////////////////////////////////////////////////////////////////
/** Input/forward iterator free functions */

template<class Iterator_l, class Iterator_r>
bool operator==(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
{
	return it_l.base() == it_r.base();
}

template<class Iterator_l, class Iterator_r>
bool operator!=(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
{
	return it_l.base() != it_r.base();
}

//////////////////////////////////////////////////////////////////////////
/** Random access iterator free functions */

template<class Iterator_l, class Iterator_r>
bool operator<(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
{
	return it_l.base() < it_r.base();
}

template<class Iterator_l, class Iterator_r>
bool operator<=(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
{
	return it_l.base() <= it_r.base();
}

template<class Iterator_l, class Iterator_r>
bool operator>(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
{
	return it_l.base() > it_r.base();
}

template<class Iterator_l, class Iterator_r>
bool operator>=(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
{
	return it_l.base() >= it_r.base();
}

template<class Iterator_l, class Iterator_r>
auto operator-(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
{
	return it_l.base() - it_r.base();
}

template<class Iterator>
auto operator+(
	typename Smart_ptr_iter<Iterator>::difference_type n, const Smart_ptr_iter<Iterator>& it)
{
	return it + n;
}
} // namespace esu
