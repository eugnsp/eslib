#pragma once
#include "check_elements.hpp"

#include <esl/dense.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>

template<typename T>
struct Block_view_lvalue_copy_constructor
{
	auto name()
	{
		return "Block view copy constructor";
	}

	void operator()()
	{
		esl::Matrix<T, 3, 3, esl::Col_major> m1{1, 2, 3, 4, 5, 6, 7, 8, 9};
		const esl::Matrix<T, 3, 3, esl::Col_major> m2{1, 2, 3, 4, 5, 6, 7, 8, 9};

		auto v1 = m1.view(1, 1, 2, 3);
		auto v1c = v1;

		static_assert(std::is_same_v<decltype(v1(0, 0)), T&>);
		static_assert(std::is_same_v<decltype(v1c(0, 0)), T&>);
		assert(v1.rows() == v1c.rows());
		assert(v1.cols() == v1c.cols());
		assert(v1.lead_dim() == v1c.lead_dim());
		assert(v1.data() == v1c.data());
	}
};

template<typename T>
struct Block_view_lvalue_size
{
	auto name()
	{
		return "Block view size";
	}

	void operator()()
	{
		esl::Matrix<T, 10, 11, esl::Col_major> msc;
		esl::Matrix<T, 10, 11, esl::Row_major> msr;

		op(msc, 0, 0, 5, 6);
		op(msc, 1, 2, 5, 6);
		op(msr, 0, 0, 5, 6);
		op(msr, 1, 2, 5, 6);

		op<0, 0, 5, 6>(msc);
		op<1, 2, 5, 6>(msc);
		op<0, 0, 5, 6>(msr);
		op<1, 2, 5, 6>(msr);

		esl::Matrix<T, esl::dynamic, esl::dynamic, esl::Col_major> mdc;
		esl::Matrix<T, esl::dynamic, esl::dynamic, esl::Row_major> mdr;

		op(mdc, 0, 0, 5, 6);
		op(mdc, 1, 2, 5, 6);
		op(mdr, 0, 0, 5, 6);
		op(mdr, 1, 2, 5, 6);

		op<0, 0, 5, 6>(mdc);
		op<1, 2, 5, 6>(mdc);
		op<0, 0, 5, 6>(mdr);
		op<1, 2, 5, 6>(mdr);
	}

	template<class M>
	void op(const M& m, std::size_t start_row, std::size_t start_col, std::size_t rows, std::size_t cols)
	{
		auto v = m.view(start_row, start_col, rows, cols);
		assert(v.rows() == rows);
		assert(v.cols() == cols);
		assert(v.lead_dim() == m.lead_dim());
		static_assert(std::is_same_v<esl::Layout_tag<decltype(v)>, esl::Layout_tag<M>>);
	}

	template<std::size_t start_row, std::size_t start_col, std::size_t rows, std::size_t cols, class M>
	void op(const M& m)
	{
		auto v = m.template view<start_row, start_col, rows, cols>();
		assert(v.rows() == rows);
		assert(v.cols() == cols);
		assert(v.lead_dim() == m.lead_dim());
		static_assert(std::is_same_v<esl::Layout_tag<decltype(v)>, esl::Layout_tag<M>>);
	}
};

template<typename T>
struct Block_view_lvalue_data
{
	auto name()
	{
		return "Block view data access";
	}

	void operator()()
	{
		esl::Matrix<T, 3, 4, esl::Col_major> m1c{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
		esl::Matrix<T, 3, 4, esl::Row_major> m1r{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
		const esl::Matrix<T, 3, 4, esl::Col_major> m2c{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

		auto v1c = m1c.view(1, 1, 2, 3);
		static_assert(std::is_same_v<decltype(v1c(0, 0)), T&>);
		static_assert(std::is_same_v<decltype(v1c.data()), T*>);
		assert(check_elements(v1c, {5, 6, 8, 9, 11, 12}));
		assert(check_elements_data(v1c, {5, 6, 8, 9, 11, 12}));

		v1c(1, 1) = 13;
		assert(m1c(2, 2) == 13);

		auto v2c = m2c.view(1, 1, 2, 3);
		static_assert(std::is_same_v<decltype(v2c(0, 0)), const T&>);
		static_assert(std::is_same_v<decltype(v2c.data()), const T*>);
		assert(check_elements(v2c, {5, 6, 8, 9, 11, 12}));
		assert(check_elements_data(v2c, {5, 6, 8, 9, 11, 12}));

		auto v1r = m1r.view(1, 1, 2, 3);
		static_assert(std::is_same_v<decltype(v1r(0, 0)), int&>);
		static_assert(std::is_same_v<decltype(v1r.data()), T*>);
		assert(check_elements(v1r, {6, 10, 7, 11, 8, 12}));
		assert(check_elements_data(v1r, {6, 10, 7, 11, 8, 12}));

		v1r(1, 1) = 13;
		assert(m1r(2, 2) == 13);
	}
};

template<typename T>
struct Block_view_lvalue_const
{
	auto name()
	{
		return "Block view const-correctness";
	}

	void operator()()
	{
		esl::Matrix<T, 3, 3> m1;
		const esl::Matrix<T, 3, 3> m1c{};

		auto v1 = m1.view(0, 0, 1, 1);
		auto v1c = m1c.view(0, 0, 1, 1);
		static_assert(std::is_same_v<decltype(v1(0, 0)), T&>);
		static_assert(std::is_same_v<decltype(v1c(0, 0)), const T&>);
		static_assert(std::is_same_v<decltype(v1.data()), T*>);
		static_assert(std::is_same_v<decltype(v1c.data()), const T*>);

		const auto v2 = m1.view(0, 0, 1, 1);
		const auto v2c = m1c.view(0, 0, 1, 1);

		static_assert(std::is_same_v<decltype(v2(0, 0)), const T&>);
		static_assert(std::is_same_v<decltype(v2c(0, 0)), const T&>);
		static_assert(std::is_same_v<decltype(v2.data()), const T*>);
		static_assert(std::is_same_v<decltype(v2c.data()), const T*>);

		auto v3 = m1.cview(0, 0, 1, 1);
		auto v3c = m1c.cview(0, 0, 1, 1);
		static_assert(std::is_same_v<decltype(v3(0, 0)), const T&>);
		static_assert(std::is_same_v<decltype(v3c(0, 0)), const T&>);
		static_assert(std::is_same_v<decltype(v3.data()), const T*>);
		static_assert(std::is_same_v<decltype(v3c.data()), const T*>);

		const auto v4 = m1.cview(0, 0, 1, 1);
		const auto v4c = m1c.cview(0, 0, 1, 1);
		static_assert(std::is_same_v<decltype(v4(0, 0)), const T&>);
		static_assert(std::is_same_v<decltype(v4c(0, 0)), const T&>);
		static_assert(std::is_same_v<decltype(v4.data()), const T*>);
		static_assert(std::is_same_v<decltype(v4c.data()), const T*>);
	}
};
