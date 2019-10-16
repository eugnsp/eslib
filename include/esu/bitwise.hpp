#pragma once
#include <climits>

namespace esu
{
#ifdef __GNUG__
inline unsigned char n_leading_zero_bits_nz(unsigned int value)
{
	return static_cast<unsigned char>(__builtin_clz(value));
}

inline unsigned char n_leading_zero_bits_nz(unsigned long value)
{
	return static_cast<unsigned char>(__builtin_clzl(value));
}

inline unsigned char n_leading_zero_bits_nz(unsigned long long value)
{
	return static_cast<unsigned char>(__builtin_clzll(value));
}

inline unsigned char n_leading_zero_bits(unsigned int value)
{
	if (value > 0)
		return n_leading_zero_bits_nz(value);
	else
		return CHAR_BIT * sizeof(unsigned int);
}

inline unsigned char n_leading_zero_bits(unsigned long value)
{
	if (value > 0)
		return n_leading_zero_bits_nz(value);
	else
		return CHAR_BIT * sizeof(unsigned long);
}

inline unsigned char n_leading_zero_bits(unsigned long long value)
{
	if (value > 0)
		return n_leading_zero_bits_nz(value);
	else
		return CHAR_BIT * sizeof(unsigned long long);
}
#endif
} // namespace esu
