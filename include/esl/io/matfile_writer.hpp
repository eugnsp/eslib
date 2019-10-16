#pragma once
#include <esl/dense.hpp>
#include <esl/io/matfile.hpp>
#include <esl/sparse.hpp>

#include <esu/time.hpp>
#include <esu/type_traits.hpp>

#include <mkl.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace esl
{
// MAT-file class that writes data to Level 5 MAT-files
class Matfile_writer
{
public:
	explicit Matfile_writer(const std::string& file_name);

	void write(const std::string& var_name, const std::string& str)
	{
		write_array_element(var_name, 1, str.length(), str.c_str());
	}

	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	void write(const std::string& var_name, T scalar)
	{
		write_array_element(var_name, 1, 1, &scalar);
	}

	template<typename T>
	void write(const std::string& var_name, const std::vector<T>& vector)
	{
		write_array_element(var_name, vector.size(), 1, vector.data());
	}

	template<class T, auto rows, auto cols>
	void write(const std::string& var_name, const Matrix<T, rows, cols>& matrix)
	{
		write_array_element(var_name, matrix.rows(), matrix.cols(), matrix.data());
	}

	template<class Symmetry_tag_type>
	void write(const std::string& var_name, const Csr_matrix<double, Symmetry_tag_type>& matrix)
	{
		const MKL_INT n = matrix.n_cols();
		auto nnz = matrix.nnz();

		const auto size_in_bytes = nnz * sizeof(double);
		if (size_in_bytes > INT32_MAX)
			throw std::length_error("Level 5 MAT-files cannot hold variables exceeding 2GB.");

		// Convert CSR to CSC first
		std::vector<MKL_INT> csc_rows(nnz);
		std::vector<MKL_INT> csc_cols(n + 1);
		std::vector<double> csc_values(nnz);

		MKL_INT job[8] = {};
		MKL_INT info;

		job[0] = 0;			 // The matrix in the CSR format is converted to the CSC format
		job[1] = job[2] = 0; // Zero-based indexing
		job[5] = 1;			 // All output arrays (acsc, ja1, and ia1) are filled in for the output storage

		mkl_dcsrcsc(job, &n, const_cast<double*>(matrix.data()), (MKL_INT*)(matrix.col_indices()),
			(MKL_INT*)(matrix.row_indices()), csc_values.data(), csc_rows.data(), csc_cols.data(), &info);

		if (info != 0)
			throw std::runtime_error("Matrix format conversion error");

		write_sparse_array_element(var_name, matrix.n_rows(), matrix.n_cols(), matrix.nnz(), csc_rows.data(),
			csc_cols.data(), csc_values.data());
	}

	void close()
	{
		file_.close();
	}

private:
	// Computes size rounded up to the 64-bit boundary (padding_size = 8)
	static std::size_t padded_size(std::size_t size);

	void write_zero_padding(std::size_t length_to_pad);

	template<typename T>
	void write_raw(const T& data)
	{
		file_.write(reinterpret_cast<const char*>(&data), sizeof(T));
	}

	template<typename T>
	void write_raw(const T* data, std::size_t n_elements)
	{
		file_.write(reinterpret_cast<const char*>(data), n_elements * sizeof(T));
	}

	void write_tag(internal::matfile::Data_types, std::size_t n_bytes);

	void write_header();

	void write_array_flags_subelement(internal::matfile::Class_types, bool is_complex, std::size_t nnz);

	void write_dimensions_subelement(std::size_t rows, std::size_t cols);

	void write_array_name_subelement(const std::string& name);

	template<typename T>
	void write_array_element(const std::string& name, std::size_t rows, std::size_t cols, const T* real_values,
		esu::Type_identity<const T*> complex_values = nullptr);

	template<typename T, typename Index>
	void write_sparse_array_element(const std::string& name, std::size_t rows, std::size_t cols, std::size_t nnz,
		const Index* ir, const Index* jc, const T* real_values, esu::Type_identity<const T*> complex_values = nullptr);

private:
	std::ofstream file_;
};

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/

template<typename T>
void Matfile_writer::write_array_element(const std::string& name, std::size_t rows, std::size_t cols,
	const T* real_values, esu::Type_identity<const T*> complex_values)
{
	assert(real_values);

	const bool is_complex_data = (complex_values != nullptr);
	const auto data_size_in_bytes = rows * cols * sizeof(T);

	if (data_size_in_bytes > INT32_MAX)
		throw std::length_error("Level 5 MAT-files cannot hold variables exceeding 2GB");

	const std::size_t total_size_in_bytes =							  // The total size includes all subelement paddings
		sizeof(internal::matfile::Array_flags) +					  // Array flags
		sizeof(internal::matfile::Dimensions) +						  // Dimensions array
		sizeof(internal::matfile::Tag) + padded_size(name.length()) + // Array name
		(sizeof(internal::matfile::Tag) + padded_size(data_size_in_bytes)) * (is_complex_data ? 2 : 1); // Data size

	write_tag(internal::matfile::Data_types::MATRIX, total_size_in_bytes);

	write_array_flags_subelement(internal::matfile::Class_type<T>::value, is_complex_data, 0);
	write_dimensions_subelement(rows, cols);
	write_array_name_subelement(name);

	// Real data
	write_tag(internal::matfile::Data_type<T>::value, data_size_in_bytes);
	write_raw(real_values, rows * cols);
	write_zero_padding(data_size_in_bytes);

	if (is_complex_data)
	{
		// Complex data
		write_tag(internal::matfile::Data_type<T>::value, data_size_in_bytes);
		write_raw(complex_values, rows * cols);
		write_zero_padding(data_size_in_bytes);
	}
}

template<typename T, typename Index>
void Matfile_writer::write_sparse_array_element(const std::string& name, std::size_t rows, std::size_t cols,
	std::size_t nnz, const Index* ir, const Index* jc, const T* real_values,
	esu::Type_identity<const T*> complex_values)
{
	static_assert(std::is_integral_v<Index>);
	assert(real_values);

	const bool is_complex_data = (complex_values != nullptr);
	const std::size_t data_size_in_bytes = nnz * sizeof(T);
	const std::size_t row_ind_size_in_bytes = 4 * nnz;
	const std::size_t column_ind_size_in_bytes = 4 * (cols + 1);

	if (data_size_in_bytes > INT32_MAX)
		throw std::length_error("Level 5 MAT-files cannot hold variables exceeding 2GB");

	const std::size_t total_size_in_bytes =							  // Total size includes all subelement paddings
		sizeof(internal::matfile::Array_flags) +					  // Array flags
		sizeof(internal::matfile::Dimensions) +						  // Dimensions array
		sizeof(internal::matfile::Tag) + padded_size(name.length()) + // Array name
		sizeof(internal::matfile::Tag) + padded_size(row_ind_size_in_bytes) +	 // Row indices
		sizeof(internal::matfile::Tag) + padded_size(column_ind_size_in_bytes) + // Column indices
		(sizeof(internal::matfile::Tag) + padded_size(data_size_in_bytes)) * (is_complex_data ? 2 : 1); // Data size

	write_tag(internal::matfile::Data_types::MATRIX, total_size_in_bytes);

	write_array_flags_subelement(internal::matfile::Class_types::SPARSE, is_complex_data, nnz);
	write_dimensions_subelement(rows, cols);
	write_array_name_subelement(name);

	// Row indices
	write_tag(internal::matfile::Data_types::INT32, row_ind_size_in_bytes);
	if constexpr (sizeof(Index) == 4)
		write_raw(ir, nnz);
	else
		for (std::size_t i = 0; i < nnz; ++i)
			write_raw(static_cast<std::uint32_t>(ir[i]));
	write_zero_padding(row_ind_size_in_bytes);

	// Column indices
	write_tag(internal::matfile::Data_types::INT32, column_ind_size_in_bytes);
	if constexpr (sizeof(Index) == 4)
		write_raw(jc, cols + 1);
	else
		for (std::size_t i = 0; i < cols + 1; ++i)
			write_raw(static_cast<std::uint32_t>(jc[i]));
	write_zero_padding(column_ind_size_in_bytes);

	// Real data
	write_tag(internal::matfile::Data_type<T>::value, data_size_in_bytes);
	write_raw(real_values, nnz);
	write_zero_padding(data_size_in_bytes);

	if (is_complex_data)
	{
		// Complex data
		write_tag(internal::matfile::Data_type<T>::value, data_size_in_bytes);
		write_raw(complex_values, nnz);
		write_zero_padding(data_size_in_bytes);
	}
}
} // namespace esl
