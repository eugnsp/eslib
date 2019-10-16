#include <cassert>
#include <cstddef>
#include <cstdint>
#include <esl/dense.hpp>
#include <esl/io/matfile_writer.hpp>
#include <esl/sparse.hpp>
#include <esu/time.hpp>
#include <esu/type_traits.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include <mkl.h>

namespace esl
{
Matfile_writer::Matfile_writer(const std::string& file_name)
{
	file_.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	file_.open(file_name, std::ofstream::binary);

	write_header();
}

// 	void write(const std::string& var_name, const std::string& str)
// 	{
// 		write_array_element(var_name, 1, str.length(), str.c_str());
// 	}
//
// 	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
// 	void write(const std::string& var_name, T scalar)
// 	{
// 		write_array_element(var_name, 1, 1, &scalar);
// 	}
//
// 	template<typename T>
// 	void write(const std::string& var_name, const std::vector<T>& vector)
// 	{
// 		write_array_element(var_name, vector.size(), 1, vector.data());
// 	}
//
// 	template<class T, std::size_t rows, std::size_t cols>
// 	void write(const std::string& var_name, const Matrix<T, rows, cols>& matrix)
// 	{
// 		write_array_element(var_name, matrix.rows(), matrix.cols(), matrix.data());
// 	}
//
// 	template<class Symmetry_tag_type>
// 	void write(const std::string& var_name, const la::Sparse_matrix<double, Symmetry_tag_type>&
// matrix)
// 	{
// 		const MKL_INT n = matrix.n_cols();
// 		auto nnz = matrix.nnz();
//
// 		const auto size_in_bytes = nnz * sizeof(double);
// 		if (size_in_bytes > INT32_MAX)
// 			throw std::length_error("Level 5 MAT-files cannot hold variables exceeding 2GB.");
//
// 		// Convert CSR to CSC first
// 		std::vector<MKL_INT> cscRows(nnz);
// 		std::vector<MKL_INT> cscColumnIndex(n + 1);
// 		std::vector<double> cscValues(nnz);
//
// 		MKL_INT job[8] = { };
// 		MKL_INT info;
//
// 		job[0] = 0;				// The matrix in the CSR format is converted to the CSC format
// 		job[1] = job[2] = 0;	// Zero-based indexing
// 		job[5] = 1;				// All output arrays (acsc, ja1, and ia1) are filled in for the output
// storage
//
// 		mkl_dcsrcsc(job, &n, const_cast<double*>(matrix.data()), (MKL_INT*)(matrix.col_indices()),
// 			(MKL_INT*)(matrix.row_indices()), cscValues.data(), cscRows.data(), cscColumnIndex.data(),
// &info);
//
// 		if (info != 0)
// 			throw std::runtime_error("Matrix format conversion error");
//
// 		write_sparse_array_element(var_name, matrix.n_rows(), matrix.n_cols(), matrix.nnz(),
// cscRows.data(), cscColumnIndex.data(), cscValues.data());
// 	}
//
// 	void close()
// 	{
// 		file_.close();
// 	}
//
// private:
// 	static constexpr std::size_t padding_size = 8;
//
// Computes size rounded up to the 64-bit boundary
std::size_t Matfile_writer::padded_size(std::size_t size)
{
	return 8 * ((size + 7) / 8);
}

void Matfile_writer::write_zero_padding(std::size_t length_to_pad)
{
	const char zero_padding_bytes[8] = {0};

	const auto n_padding_bytes = padded_size(length_to_pad) - length_to_pad;
	write_raw(zero_padding_bytes, n_padding_bytes);
}

// 	template<typename T>
// 	void write_raw(const T& data)
// 	{
// 		file_.write(reinterpret_cast<const char*>(&data), sizeof(T));
// 	}
//
// 	template<typename T>
// 	void write_raw(const T* data, std::size_t n_elements)
// 	{
// 		file_.write(reinterpret_cast<const char*>(data), n_elements * sizeof(T));
// 	}

void Matfile_writer::write_tag(internal::matfile::Data_types data_type, std::size_t n_bytes)
{
	internal::matfile::Tag tag{data_type, static_cast<std::uint32_t>(n_bytes)};
	write_raw(tag);
}

void Matfile_writer::write_header()
{
	internal::matfile::Header header;
	header.reserved1 = 0;
	header.reserved2 = 0;
	header.version = 0x0100;
	header.endian = ('M' << 8) | 'I'; // = "IM"

	std::string text("MATLAB 5.0 MAT-file, created on: " + esu::date_time_string());
	text.resize(sizeof(header.text), ' ');
	text.copy(header.text, sizeof(header.text));

	write_raw(header);
}

void Matfile_writer::write_array_flags_subelement(
	internal::matfile::Class_types class_type, bool is_complex, std::size_t nnz)
{
	[[maybe_unused]] constexpr std::uint8_t logical_flag = 0b0010;
	[[maybe_unused]] constexpr std::uint8_t global_flag = 0b0100;
	[[maybe_unused]] constexpr std::uint8_t complex_flag = 0b1000;

	internal::matfile::Array_flags array_flags;
	array_flags.class_type = static_cast<std::uint8_t>(class_type);
	array_flags.nnz = static_cast<std::uint32_t>(nnz);
	array_flags.flags = 0;
	if (is_complex)
		array_flags.flags |= complex_flag;

	write_raw(array_flags);
}

void Matfile_writer::write_dimensions_subelement(std::size_t rows, std::size_t cols)
{
	internal::matfile::Dimensions dims;
	dims.rows = static_cast<std::uint32_t>(rows);
	dims.cols = static_cast<std::uint32_t>(cols);

	write_raw(dims);
}

void Matfile_writer::write_array_name_subelement(const std::string& name)
{
	write_tag(internal::matfile::Data_types::INT8, name.length());
	write_raw(name.c_str(), name.length());
	write_zero_padding(name.length());
}

// 	template<typename T>
// 	void write_array_element(
// 		const std::string& name,
// 		std::size_t rows, std::size_t cols,
// 		const T* real_values,
// 		esu::Identity_t<const T*> complex_values = nullptr);
//
// 	template<typename T, typename Index>
// 	void write_sparse_array_element(
// 		const std::string& name,
// 		std::size_t rows, std::size_t cols,
// 		std::size_t nnz,
// 		const Index* ir, const Index* jc,
// 		const T* real_values,
// 		esu::Identity_t<const T*> complex_values = nullptr);
//
// private:
// 	std::ofstream file_;
// };
//
// /************************************************************************/
// /* IMPLEMENTATION                                                       */
// /************************************************************************/
//
// template<typename T>
// void Matfile_writer::write_array_element(const std::string& name,
// 	std::size_t rows, std::size_t cols,
// 	const T* real_values,
// 	esu::Identity_t<const T*> complex_values)
// {
// 	assert(real_values);
//
// 	const bool is_complex_data = (complex_values != nullptr);
// 	const auto data_size_in_bytes = rows * cols * sizeof(T);
//
// 	if (data_size_in_bytes > INT32_MAX)
// 		throw std::length_error("Level 5 MAT-files cannot hold variables exceeding 2GB");
//
// 	const std::size_t total_size_in_bytes =	// The total size includes all subelement paddings
// 		sizeof(Array_flags) +						// Array flags
// 		sizeof(Dimensions) +						// Dimensions array
// 		sizeof(Tag) + padded_size(name.length()) +	// Array name
// 		(sizeof(Tag) + padded_size(data_size_in_bytes)) *
// 		(is_complex_data ? 2 : 1);					// Data size
//
// 	write_tag(internal::Matfile_data_types::MATRIX, total_size_in_bytes);
//
// 	write_array_flags_subelement(internal::Matfile_class_type<T>::value, is_complex_data, 0);
// 	write_dimensions_subelement(rows, cols);
// 	write_array_name_subelement(name);
//
// 	// Real data
// 	write_tag(internal::Matfile_data_type<T>::value, data_size_in_bytes);
// 	write_raw(real_values, rows * cols);
// 	write_zero_padding(data_size_in_bytes);
//
// 	if (is_complex_data)
// 	{
// 		// Complex data
// 		write_tag(internal::Matfile_data_type<T>::value, data_size_in_bytes);
// 		write_raw(complex_values, rows * cols);
// 		write_zero_padding(data_size_in_bytes);
// 	}
// }
//
// template<typename T, typename Index>
// void Matfile_writer::write_sparse_array_element(const std::string& name,
// 	std::size_t rows, std::size_t cols,
// 	std::size_t nnz,
// 	const Index* ir, const Index* jc,
// 	const T* real_values,
// 	esu::Identity_t<const T*> complex_values)
// {
// 	static_assert(std::is_integral_v<Index>);
// 	assert(real_values);
//
// 	const bool is_complex_data = (complex_values != nullptr);
// 	const std::size_t data_size_in_bytes = nnz * sizeof(T);
// 	const std::size_t row_ind_size_in_bytes = 4 * nnz;
// 	const std::size_t column_ind_size_in_bytes = 4 * (cols + 1);
//
// 	if (data_size_in_bytes > INT32_MAX)
// 		throw std::length_error("Level 5 MAT-files cannot hold variables exceeding 2GB");
//
// 	const std::size_t total_size_in_bytes =		// Total size includes all subelement paddings
// 		sizeof(Array_flags) +									// Array flags
// 		sizeof(Dimensions) +									// Dimensions array
// 		sizeof(Tag) + padded_size(name.length()) +				// Array name
// 		sizeof(Tag) + padded_size(row_ind_size_in_bytes) +		// Row indices
// 		sizeof(Tag) + padded_size(column_ind_size_in_bytes) +	// Column indices
// 		(sizeof(Tag) + padded_size(data_size_in_bytes)) *
// 		(is_complex_data ? 2 : 1);								// Data size
//
// 	write_tag(internal::Matfile_data_types::MATRIX, total_size_in_bytes);
//
// 	write_array_flags_subelement(internal::Matfile_class_types::SPARSE, is_complex_data, nnz);
// 	write_dimensions_subelement(rows, cols);
// 	write_array_name_subelement(name);
//
// 	// Row indices
// 	write_tag(internal::Matfile_data_types::INT32, row_ind_size_in_bytes);
// 	if constexpr (sizeof(Index) == 4)
// 		write_raw(ir, nnz);
// 	else
// 		for (std::size_t i = 0; i < nnz; ++i)
// 			write_raw(static_cast<std::uint32_t>(ir[i]));
// 	write_zero_padding(row_ind_size_in_bytes);
//
// 	// Column indices
// 	write_tag(internal::Matfile_data_types::INT32, column_ind_size_in_bytes);
// 	if constexpr (sizeof(Index) == 4)
// 		write_raw(jc, cols + 1);
// 	else
// 		for (std::size_t i = 0; i < cols + 1; ++i)
// 			write_raw(static_cast<std::uint32_t>(jc[i]));
// 	write_zero_padding(column_ind_size_in_bytes);
//
// 	// Real data
// 	write_tag(internal::Matfile_data_type<T>::value, data_size_in_bytes);
// 	write_raw(real_values, nnz);
// 	write_zero_padding(data_size_in_bytes);
//
// 	if (is_complex_data)
// 	{
// 		// Complex data
// 		write_tag(internal::Matfile_data_type<T>::value, data_size_in_bytes);
// 		write_raw(complex_values, nnz);
// 		write_zero_padding(data_size_in_bytes);
// 	}
// }
} // namespace esl
