#pragma once
#include <cstdint>

namespace esl::internal::matfile
{
enum class Data_types : std::uint32_t
{
	INT8 = 1,
	UINT8 = 2,
	INT16 = 3,
	UINT16 = 4,
	INT32 = 5,
	UINT32 = 6,
	INT64 = 12,
	UINT64 = 13,
	SINGLE = 7,
	DOUBLE = 9,
	MATRIX = 14,
	COMPRESSED = 15,
	UTF8 = 16,
	UTF16 = 17,
	UTF32 = 18
};

template<typename>
struct Data_type;

// clang-format off
template<> struct Data_type<char> 			{ static constexpr auto value = Data_types::UTF8;   };
template<> struct Data_type<signed char> 	{ static constexpr auto value = Data_types::INT8;   };
template<> struct Data_type<unsigned char> 	{ static constexpr auto value = Data_types::UINT8;  };
template<> struct Data_type<std::int16_t> 	{ static constexpr auto value = Data_types::INT16;  };
template<> struct Data_type<std::uint16_t> 	{ static constexpr auto value = Data_types::UINT16; };
template<> struct Data_type<std::int32_t> 	{ static constexpr auto value = Data_types::INT32;  };
template<> struct Data_type<std::uint32_t> 	{ static constexpr auto value = Data_types::UINT32; };
template<> struct Data_type<std::int64_t> 	{ static constexpr auto value = Data_types::INT64;  };
template<> struct Data_type<std::uint64_t> 	{ static constexpr auto value = Data_types::UINT64; };
template<> struct Data_type<float> 			{ static constexpr auto value = Data_types::SINGLE; };
template<> struct Data_type<double> 		{ static constexpr auto value = Data_types::DOUBLE; };
// clang-format on

// MAT-file class types
enum class Class_types : std::uint8_t
{
	CELL = 1,
	STRUCT = 2,
	OBJECT = 3,
	CHAR = 4,
	SPARSE = 5,
	DOUBLE = 6,
	SINGLE = 7,
	INT8 = 8,
	UINT8 = 9,
	INT16 = 10,
	UINT16 = 11,
	INT32 = 12,
	UINT32 = 13,
	INT64 = 14,
	UINT64 = 15
};

template<typename>
struct Class_type;

// clang-format off
template<> struct Class_type<std::int8_t> 	{ static constexpr auto value = Class_types::INT8;   };
template<> struct Class_type<std::uint8_t> 	{ static constexpr auto value = Class_types::UINT8;  };
template<> struct Class_type<std::int16_t> 	{ static constexpr auto value = Class_types::INT16;  };
template<> struct Class_type<std::uint16_t> { static constexpr auto value = Class_types::UINT16; };
template<> struct Class_type<std::int32_t>	{ static constexpr auto value = Class_types::INT32;  };
template<> struct Class_type<std::uint32_t>	{ static constexpr auto value = Class_types::UINT32; };
template<> struct Class_type<std::int64_t>	{ static constexpr auto value = Class_types::INT64;  };
template<> struct Class_type<std::uint64_t>	{ static constexpr auto value = Class_types::UINT64; };
template<> struct Class_type<float> 		{ static constexpr auto value = Class_types::SINGLE; };
template<> struct Class_type<double>		{ static constexpr auto value = Class_types::DOUBLE; };
template<> struct Class_type<char> 			{ static constexpr auto value = Class_types::CHAR;   };
// clang-format on

#pragma pack(push, 1)
struct Header
{
	char text[116];
	std::uint32_t reserved1;
	std::uint32_t reserved2;
	std::uint16_t version;
	std::uint16_t endian;
};

struct Tag
{
	Data_types data_type;
	std::uint32_t n_bytes;
};

struct Array_flags
{
	const Tag tag = {Data_types::UINT32, 8};
	std::uint8_t class_type;
	std::uint8_t flags;
	const std::uint16_t reserved = 0;
	std::uint32_t nnz;
};

struct Dimensions
{
	const Tag tag = {Data_types::INT32, 8};
	std::uint32_t rows;
	std::uint32_t cols;
};
#pragma pack(pop)

struct Flags
{
	bool is_complex;
	bool is_global;
	bool is_logical;

	void from_uint8(std::uint8_t flags)
	{
		is_complex = flags & complex_flag;
		is_global = flags & global_flag;
		is_logical = flags & logical_flag;
	}

	std::uint8_t to_uint8()
	{
		std::uint8_t flags = 0;
		if (is_complex)
			flags |= complex_flag;
		if (is_global)
			flags |= global_flag;
		if (is_logical)
			flags |= logical_flag;
		return flags;
	}

private:
	static constexpr std::uint8_t logical_flag = 0b0010;
	static constexpr std::uint8_t global_flag = 0b0100;
	static constexpr std::uint8_t complex_flag = 0b1000;
};
} // namespace esl::internal::matfile
