// #pragma once
// #include "../config.hpp"
// #include "expression.hpp"
// #include <cassert>
// #include <cstddef>
// #include <utility>

// namespace esl::internal
// {
// template<class T_Derived, std::size_t t_rows, std::size_t t_cols>
// class Extended : public Dense<T_Derived>
// {
// public:
// 	static constexpr std::size_t rows();
// 	static constexpr std::size_t cols();
// 	static constexpr std::size_t size();

// protected:
// 	constexpr Extended() = default;

// 	// rows and cols are dummy arguments that should be equal to t_rows and t_cols
// 	constexpr Extended(std::size_t rows, std::size_t cols);

// private:
// 	static constexpr std::size_t rows_ = t_rows;
// 	static constexpr std::size_t cols_ = t_cols;
// };

// template<class T_Derived, std::size_t t_rows>
// class Extended<T_Derived, t_rows, 0> : public Dense<T_Derived>
// {
// public:
// 	static constexpr std::size_t rows();
// 	std::size_t cols() const;

// 	void set_size(std::size_t cols);

// 	void swap(Extended& other) noexcept;

// protected:
// 	Extended() = default;

// 	// rows is a dummy argument that should be equal to t_rows
// 	Extended(std::size_t rows, std::size_t cols);

// private:
// 	static constexpr std::size_t rows_ = t_rows;
// 	std::size_t cols_ = 0;
// };

// template<class T_Derived, std::size_t t_cols>
// class Extended<T_Derived, 0, t_cols> : public Dense<T_Derived>
// {
// public:
// 	std::size_t rows() const;
// 	static constexpr std::size_t cols();

// 	void set_size(std::size_t rows);

// 	void swap(Extended& other) noexcept;

// protected:
// 	Extended() = default;

// 	// cols is a dummy argument that should be equal to t_cols
// 	Extended(std::size_t rows, std::size_t cols);

// private:
// 	std::size_t rows_ = 0;
// 	static constexpr std::size_t cols_ = t_cols;
// };

// template<class T_Derived>
// class Extended<T_Derived, 0, 0> : public Dense<T_Derived>
// {
// public:
// 	std::size_t rows() const;
// 	std::size_t cols() const;

// 	void set_size(std::size_t rows, std::size_t cols);

// 	void swap(Extended& other) noexcept;

// protected:
// 	Extended() = default;
// 	Extended(std::size_t rows, std::size_t cols);

// private:
// 	std::size_t rows_ = 0;
// 	std::size_t cols_ = 0;
// };

// /************************************************************************/
// /* IMPLEMENTATION                                                       */
// /************************************************************************/

// template<class T_Derived, std::size_t t_rows, std::size_t t_cols>
// constexpr std::size_t Extended<T_Derived, t_rows, t_cols>::rows()
// {
// 	return t_rows;
// }

// template<class T_Derived, std::size_t t_rows, std::size_t t_cols>
// constexpr std::size_t Extended<T_Derived, t_rows, t_cols>::cols()
// {
// 	return t_cols;
// }

// template<class T_Derived, std::size_t t_rows, std::size_t t_cols>
// constexpr std::size_t Extended<T_Derived, t_rows, t_cols>::size()
// {
// 	return rows() * cols();
// }

// template<class T_Derived, std::size_t t_rows, std::size_t t_cols>
// constexpr Extended<T_Derived, t_rows, t_cols>::Extended(std::size_t rows, std::size_t cols)
// {
// 	MATHLA_ASSERT(rows == t_rows && cols == t_cols);
// }

// //////////////////////////////////////////////////////////////////////////

// template<class T_Derived, std::size_t t_rows>
// constexpr std::size_t Extended<T_Derived, t_rows, 0>::rows()
// {
// 	return t_rows;
// }

// template<class T_Derived, std::size_t t_rows>
// std::size_t Extended<T_Derived, t_rows, 0>::cols() const
// {
// 	return cols_;
// }

// template<class T_Derived, std::size_t t_rows>
// void Extended<T_Derived, t_rows, 0>::set_size(std::size_t cols)
// {
// 	cols_ = cols;
// }

// template<class T_Derived, std::size_t t_rows>
// inline void Extended<T_Derived, t_rows, 0>::swap(Extended& other) noexcept
// {
// 	std::swap(cols_, other.cols_);
// }

// template<class T_Derived, std::size_t t_rows>
// inline Extended<T_Derived, t_rows, 0>::Extended(std::size_t rows, std::size_t cols) : cols_(cols)
// {
// 	MATHLA_ASSERT(rows == t_rows);
// }

// //////////////////////////////////////////////////////////////////////////

// template<class T_Derived, std::size_t t_cols>
// inline std::size_t Extended<T_Derived, 0, t_cols>::rows() const
// {
// 	return rows_;
// }

// template<class T_Derived, std::size_t t_cols>
// inline constexpr std::size_t Extended<T_Derived, 0, t_cols>::cols()
// {
// 	return t_cols;
// }

// template<class T_Derived, std::size_t t_cols>
// inline void Extended<T_Derived, 0, t_cols>::set_size(std::size_t rows)
// {
// 	rows_ = rows;
// }

// template<class T_Derived, std::size_t t_cols>
// inline void Extended<T_Derived, 0, t_cols>::swap(Extended& other) noexcept
// {
// 	std::swap(rows_, other.rows_);
// }

// template<class T_Derived, std::size_t t_cols>
// inline Extended<T_Derived, 0, t_cols>::Extended(std::size_t rows, std::size_t cols) : rows_(rows)
// {
// 	MATHLA_ASSERT(cols == t_cols);
// }

// //////////////////////////////////////////////////////////////////////////

// template<class T_Derived>
// std::size_t Extended<T_Derived, 0, 0>::rows() const
// {
// 	return rows_;
// }

// template<class T_Derived>
// std::size_t Extended<T_Derived, 0, 0>::cols() const
// {
// 	return cols_;
// }

// template<class T_Derived>
// inline void Extended<T_Derived, 0, 0>::set_size(std::size_t rows, std::size_t cols)
// {
// 	rows_ = rows;
// 	cols_ = cols;
// }

// template<class T_Derived>
// inline void Extended<T_Derived, 0, 0>::swap(Extended& other) noexcept
// {
// 	std::swap(rows_, other.rows_);
// 	std::swap(cols_, other.cols_);
// }

// template<class T_Derived>
// inline Extended<T_Derived, 0, 0>::Extended(std::size_t rows, std::size_t cols) : rows_(rows), cols_(cols)
// {}
// } // namespace esl::internal
