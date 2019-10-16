#pragma once
#include <iterator>
#include <type_traits>
#include <utility>

namespace esu
{
template<class Iterator, class Unary_operation>
class Transform_iterator
{
private:
	using Op = std::decay_t<Unary_operation>;
	using Traits = std::iterator_traits<Iterator>;

public:
	using iterator_category = typename Traits::iterator_category;
	using reference = std::invoke_result_t<Op, typename Traits::reference>;
	using value_type = std::remove_reference_t<reference>;
	using difference_type = typename Traits::difference_type;
	using pointer = void;

public:
	Transform_iterator() = default;

	Transform_iterator(Iterator base, Op op) : op_(std::move(op)), base_(std::move(base))
	{}

	const Op& functor() const
	{
		return op_;
	}

	// Returns the underlying iterator
	const Iterator& base() const
	{
		return base_;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Input/forward iterator members */

	reference operator*() const
	{
		return op_(*base_);
	}

	Transform_iterator& operator++()
	{
		++base_;
		return *this;
	}

	Transform_iterator operator++(int)
	{
		auto old_iter{*this};
		++base_;
		return old_iter;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Bidirectional iterator members */

	Transform_iterator& operator--()
	{
		--base_;
		return *this;
	}

	Transform_iterator operator--(int)
	{
		auto old_iter{*this};
		--base_;
		return old_iter;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Random access iterator members */

	Transform_iterator& operator+=(difference_type n)
	{
		base_ += n;
		return *this;
	}

	Transform_iterator operator+(difference_type n) const
	{
		return Transform_iterator{base_ + n, op_};
	}

	Transform_iterator& operator-=(difference_type n)
	{
		base_ -= n;
		return *this;
	}

	Transform_iterator operator-(difference_type n) const
	{
		return Transform_iterator{base_ - n, op_};
	}

	reference operator[](difference_type n) const
	{
		return op_(*base_[n]);
	}

private:
	Op op_;
	Iterator base_;
};

template<class Iterator, class Unary_operation>
Transform_iterator(Iterator, Unary_operation)->Transform_iterator<Iterator, Unary_operation>;

//////////////////////////////////////////////////////////////////////////
/** Input/forward iterator free functions */

template<class Iterator1, class Iterator2, class Unary_operation>
bool operator==(
	const Transform_iterator<Iterator1, Unary_operation>& it1,
	const Transform_iterator<Iterator2, Unary_operation>& it2)
{
	return it1.base() == it2.base();
}

template<class Iterator1, class Iterator2, class Unary_operation>
bool operator!=(
	const Transform_iterator<Iterator1, Unary_operation>& it1,
	const Transform_iterator<Iterator2, Unary_operation>& it2)
{
	return it1.base() != it2.base();
}

// template<class Iterator_l, class Iterator_r>
// bool operator!=(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
//{
//	return it_l.base() != it_r.base();
//}
//
////////////////////////////////////////////////////////////////////////////
///** Random access iterator free functions */
//
// template<class Iterator_l, class Iterator_r>
// bool operator<(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
//{
//	return it_l.base() < it_r.base();
//}
//
// template<class Iterator_l, class Iterator_r>
// bool operator<=(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
//{
//	return it_l.base() <= it_r.base();
//}
//
// template<class Iterator_l, class Iterator_r>
// bool operator>(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
//{
//	return it_l.base() > it_r.base();
//}
//
// template<class Iterator_l, class Iterator_r>
// bool operator>=(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
//{
//	return it_l.base() >= it_r.base();
//}
//
// template<class Iterator_l, class Iterator_r>
// auto operator-(const Smart_ptr_iter<Iterator_l>& it_l, const Smart_ptr_iter<Iterator_r>& it_r)
//{
//	return it_l.base() - it_r.base();
//}
//
// template<class Iterator>
// auto operator+(typename Smart_ptr_iter<Iterator>::difference_type n,
//			   const Smart_ptr_iter<Iterator>& it)
//{
//	return it + n;
//}

} // namespace esu
