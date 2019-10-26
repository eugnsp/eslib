#pragma once
#include "check_elements.hpp"

#include <esl/dense.hpp>

#include <cassert>
#include <cstddef>
#include <utility>
#include <functional>

template<typename T>
struct Matrix_default_construction
{
	auto name()
	{
		return "Matrix default construction: Matrix<...> m;";
	}

	void operator()()
	{
		op<0, 0>();
		op<0, 1>();
		op<1, 0>();
		op<1, 1>();
		op<2, 3>();
		op<10, 11>();
	}

	template<std::size_t rows, std::size_t cols>
	void op()
	{
		op_static<rows, cols, esl::Row_major>();
		op_static<rows, cols, esl::Col_major>();

		op_static<rows, cols, esl::Row_major>();
		op_static<rows, cols, esl::Col_major>();
	}

	template<std::size_t rows, std::size_t cols, class Layout>
	void op_static()
	{
		esl::Matrix<T, rows, cols, Layout> m;
		static_assert(m.rows() == rows);
		static_assert(m.cols() == cols);
		static_assert(m.size() == rows * cols);
		static_assert(m.capacity() == rows * cols);
	}

	template<std::size_t rows, std::size_t cols, class Layout>
	void op_dynamic()
	{
		esl::Matrix<T, rows, esl::dynamic, Layout> msd;
		assert(msd.rows() == rows);
		assert(msd.cols() == 0);
		assert(msd.size() == 0);

		esl::Matrix<T, esl::dynamic, cols, Layout> mds;
		assert(mds.rows() == 0);
		assert(mds.cols() == cols);
		assert(mds.size() == 0);

		esl::Matrix<T, esl::dynamic, esl::dynamic, Layout> mdd;
		assert(mdd.rows() == 0);
		assert(mdd.cols() == 0);
		assert(mdd.size() == 0);

		const esl::Matrix<T, esl::dynamic, esl::dynamic> mddc;
		assert(mddc.rows() == 0);
		assert(mddc.cols() == 0);
		assert(mddc.size() == 0);
	}
};

template<typename T>
struct Matrix_value_construction
{
	auto name()
	{
		return "Matrix value construction: Matrix<...> m{};";
	}

	void operator()()
	{
		op<1, 1>();
		op<2, 3>();
		op<10, 11>();
	}

	template<std::size_t rows, std::size_t cols>
	void op()
	{
		op2<rows, cols, esl::Row_major>();
		op2<rows, cols, esl::Col_major>();

		op2<rows, cols, esl::Row_major>();
		op2<rows, cols, esl::Col_major>();
	}

	template<std::size_t rows, std::size_t cols, class Layout>
	void op2()
	{
		esl::Matrix<T, rows, cols, Layout> mv{};
		static_assert(mv.rows() == rows);
		static_assert(mv.cols() == cols);
		static_assert(mv.size() == rows * cols);
		static_assert(mv.capacity() == rows * cols);
		check_elements(mv, 0);

		const esl::Matrix<T, rows, cols, Layout> mc{};
		static_assert(mc.rows() == rows);
		static_assert(mc.cols() == cols);
		static_assert(mc.size() == rows * cols);
		check_elements(mc, 0);
	}
};

template<typename T>
struct Matrix_construction_size
{
	auto name()
	{
		return "Matrix construction: Matrix<...> m(<size>);";
	}

	void operator()()
	{
		op<1, 1>();
		op<2, 3>();
		op<10, 11>();
	}

	template<std::size_t rows, std::size_t cols>
	void op()
	{
		op2<rows, cols, esl::Row_major>();
		op2<rows, cols, esl::Col_major>();
	}

	template<std::size_t rows, std::size_t cols, class Layout>
	void op2()
	{
		esl::Matrix<T, rows, esl::dynamic, Layout> msd(cols);
		assert(msd.rows() == rows);
		assert(msd.cols() == cols);
		assert(msd.size() == rows * cols);
		assert(msd.capacity() >= rows * cols);

		esl::Matrix<T, esl::dynamic, cols, Layout> mds(rows);
		assert(mds.rows() == rows);
		assert(mds.cols() == cols);
		assert(mds.size() == rows * cols);
		assert(mds.capacity() >= rows * cols);

		esl::Matrix<T, esl::dynamic, esl::dynamic, Layout> mdd(rows, cols);
		assert(mdd.rows() == rows);
		assert(mdd.cols() == cols);
		assert(mdd.size() == rows * cols);
		assert(mdd.capacity() >= rows * cols);
	}
};

template<typename T>
struct Matrix_construction_value
{
	auto name()
	{
		return "Matrix construction: Matrix<...> m([<size>,] value);";
	}

	void operator()()
	{
		op<1, 1>(11);
		op<2, 2>(12);
		op<2, 3>(13);
		op<10, 11>(14);
	}

	template<std::size_t rows, std::size_t cols>
	void op(T value)
	{
		op2<rows, cols, esl::Row_major>(value);
		op2<rows, cols, esl::Col_major>(value);
	}

	template<std::size_t rows, std::size_t cols, class Layout>
	void op2(T value)
	{
		esl::Matrix<T, rows, cols> mss(value);
		assert(check_elements(mss, value));

		esl::Matrix<T, rows, esl::dynamic> msd(cols, value);
		assert(check_elements(msd, value));

		esl::Matrix<T, esl::dynamic, cols> mds(rows, value);
		assert(check_elements(mds, value));

		esl::Matrix<T, esl::dynamic, esl::dynamic> mdd(rows, cols, value);
		assert(check_elements(mdd, value));
	}
};

template<typename T>
struct Matrix_construction_values
{
	auto name()
	{
		return "Matrix construction: Matrix<...> m(values...);";
	}

	void operator()()
	{
		op<1, 1>(1);
		op<2, 2>(1, 2, 3, 4);
		op<2, 3>(1, 2, 3, 4, 5, 6);
		op<3, 2>(1, 2, 3, 4, 5, 6);
	}

	template<std::size_t rows, std::size_t cols, typename... Ts>
	void op(Ts... list)
	{
		op2<rows, cols, esl::Col_major>(list...);
		op2<rows, cols, esl::Row_major>(list...);
	}

	template<std::size_t rows, std::size_t cols, class Layout, typename... Ts>
	void op2(Ts... list)
	{
		esl::Matrix<T, rows, cols, Layout> mss{list...};
		assert(check_matrix_elements(mss, {list...}));
	}
};

template<typename T>
struct Matrix_construction_init_list
{
	auto name()
	{
		return "Matrix construction: Matrix<...> m(<size>, {values...});";
	}

	void operator()()
	{
		op<1, 1>(1);
		op<2, 2>(1, 2, 3, 4);
		op<2, 3>(1, 2, 3, 4, 5, 6);
		op<3, 2>(1, 2, 3, 4, 5, 6);
	}

	template<std::size_t rows, std::size_t cols, typename... Ts>
	void op(Ts... list)
	{
		op2<rows, cols, esl::Col_major>(list...);
		op2<rows, cols, esl::Row_major>(list...);
	}

	template<std::size_t rows, std::size_t cols, class Layout, typename... Ts>
	void op2(Ts... list)
	{
		esl::Matrix<T, rows, esl::dynamic, Layout> msd(cols, {list...});
		assert(check_matrix_elements(msd, {list...}));

		esl::Matrix<T, rows, esl::dynamic, Layout> msd2({list...});
		assert(check_matrix_elements(msd2, {list...}));

		esl::Matrix<T, esl::dynamic, cols, Layout> mds(rows, {list...});
		assert(check_matrix_elements(mds, {list...}));

		esl::Matrix<T, esl::dynamic, cols, Layout> mds2({list...});
		assert(check_matrix_elements(mds2, {list...}));

		esl::Matrix<T, esl::dynamic, esl::dynamic, Layout> mdd(rows, cols, {list...});
		assert(check_matrix_elements(mdd, {list...}));
	}
};

template<typename T>
struct Matrix_copy_construction
{
	auto name()
	{
		return "Matrix copy construction";
	}

	void operator()()
	{
		op<2, 2>(1, 2, 3, 4);
		op<2, 3>(1, 2, 3, 4, 5, 6);
		op<3, 2>(1, 2, 3, 4, 5, 6);
	}

	template<std::size_t rows, std::size_t cols, typename... Ts>
	void op(Ts... list)
	{
		op2<rows, cols, esl::Col_major>(list...);
		op2<rows, cols, esl::Row_major>(list...);
	}

	template<std::size_t rows, std::size_t cols, class Layout, typename... Ts>
	void op2(Ts... list)
	{
		esl::Matrix<T, rows, cols, Layout> mss{list...};
		esl::Matrix<T, rows, cols, Layout> mssc(mss);
		assert(check_matrix_elements(mssc, {list...}));

		esl::Matrix<T, rows, esl::dynamic, Layout> msd(cols, {list...});
		esl::Matrix<T, rows, esl::dynamic, Layout> msdc(msd);
		assert(check_matrix_elements(msdc, {list...}));

		esl::Matrix<T, esl::dynamic, cols, Layout> mds(rows, {list...});
		esl::Matrix<T, esl::dynamic, cols, Layout> mdsc(mds);
		assert(check_matrix_elements(mdsc, {list...}));

		esl::Matrix<T, esl::dynamic, esl::dynamic, Layout> mdd(rows, cols, {list...});
		esl::Matrix<T, esl::dynamic, esl::dynamic, Layout> mddc(mdd);
		assert(check_matrix_elements(mddc, {list...}));
	}
};

template<typename T>
struct Matrix_move_construction
{
	auto name()
	{
		return "Matrix move construction";
	}

	void operator()()
	{
		op<2, 2>(1, 2, 3, 4);
		op<2, 3>(1, 2, 3, 4, 5, 6);
		op<3, 2>(1, 2, 3, 4, 5, 6);
	}

	template<std::size_t rows, std::size_t cols, typename... Ts>
	void op(Ts... list)
	{
		op2<rows, cols, esl::Col_major>(list...);
		op2<rows, cols, esl::Row_major>(list...);
	}

	template<std::size_t rows, std::size_t cols, class Layout, typename... Ts>
	void op2(Ts... list)
	{
		esl::Matrix<T, rows, esl::dynamic> msd(cols, {list...});
		auto dsd = msd.data();
		esl::Matrix<T, rows, esl::dynamic> msdm(std::move(msd));
		assert(msd.data() == nullptr);
		assert(msdm.data() == dsd);
		assert(check_matrix_elements(msdm, {list...}));

		esl::Matrix<T, esl::dynamic, cols> mds(rows, {list...});
		auto dds = mds.data();
		esl::Matrix<T, esl::dynamic, cols> mdsm(std::move(mds));
		assert(mds.data() == nullptr);
		assert(mdsm.data() == dds);
		assert(check_matrix_elements(mdsm, {list...}));

		esl::Matrix<T, esl::dynamic, esl::dynamic> mdd(rows, cols, {list...});
		auto ddd = mdd.data();
		esl::Matrix<T, esl::dynamic, esl::dynamic> mddm(std::move(mdd));
		assert(mdd.data() == nullptr);
		assert(mddm.data() == ddd);
		assert(check_matrix_elements(mddm, {list...}));
	}
};

template<typename T>
struct Matrix_construction_expr
{
	auto name()
	{
		return "Matrix construction: Matrix<...> m(<expression>);";
	}

	void operator()()
	{
		op<2, 2>(1, 2, 3, 4);
		op<2, 3>(1, 2, 3, 4, 5, 6);
		op<3, 2>(1, 2, 3, 4, 5, 6);
	}

	template<std::size_t rows, std::size_t cols, typename... Ts>
	void op(Ts... list)
	{
		op2<rows, cols, esl::Col_major>(std::plus<>{}, 3, 5, list...);
		op2<rows, cols, esl::Row_major>(std::plus<>{}, 3, 5, list...);

		op2<rows, cols, esl::Col_major>(std::minus<>{}, 3, 5, list...);
		op2<rows, cols, esl::Row_major>(std::minus<>{}, 3, 5, list...);
	}

	template<std::size_t rows, std::size_t cols, class Layout, class Fn, typename A, typename B, typename... Ts>
	void op2(Fn fn, A a, B b, Ts... list)
	{
		const esl::Matrix<T, rows, cols, Layout> m1{(a * list)...};
		const esl::Matrix<T, rows, cols, Layout> m2{(b * list)...};

		const auto expr = fn(m1, m2);
		const auto ab = fn(a, b);

		esl::Matrix<T, rows, cols, Layout> mss(expr);
		assert(check_matrix_elements(mss, {(ab * list)...}));

		esl::Matrix<T, rows, esl::dynamic, Layout> msd(expr);
		assert(msd.rows() == rows);
		assert(msd.cols() == cols);
		assert(check_matrix_elements(msd, {(ab * list)...}));

		esl::Matrix<T, esl::dynamic, cols, Layout> mds(expr);
		assert(mds.rows() == rows);
		assert(mds.cols() == cols);
		assert(check_matrix_elements(mds, {(ab * list)...}));

		esl::Matrix<T, esl::dynamic, esl::dynamic, Layout> mdd(expr);
		assert(mdd.rows() == rows);
		assert(mdd.cols() == cols);
		assert(check_matrix_elements(mdd, {(ab * list)...}));
	}
};

template<typename T>
struct Matrix_deduction_guides
{
	auto name()
	{
		return "Matrix deduction guides";
	}

	void operator()()
	{
		esl::Matrix<T, 3, 3> m1{1, 2, 3, 4, 5, 6, 7, 8, 9};
		esl::Matrix m2(2 * m1);
		static_assert(std::is_same_v<esl::Value_type<decltype(m2)>, T>);
		static_assert(m2.rows() == 3 && m2.rows() == 3);
		assert(check_elements(m2, {2, 4, 6, 8, 10, 12, 14, 16, 18}));

		esl::Matrix<T, 3, 3> m3{1, 2, 3, 4, 5, 6, 7, 8, 9};
		esl::Matrix m4(2 * m3);
		static_assert(std::is_same_v<esl::Value_type<decltype(m4)>, T>);
		assert(m4.rows() == 3 && m4.rows() == 3);
		assert(check_elements(m4, {2, 4, 6, 8, 10, 12, 14, 16, 18}));
	}
};
