#pragma once

namespace esu
{
// Returns the determinant of a 2-by-2 matrix
template<typename T>
T det(T a00, T a10, T a01, T a11)
{
	return a00 * a11 - a01 * a10;
}

// Returns the determinant of a 3-by-3 matrix
template<typename T>
T det(T a00, T a10, T a20, T a01, T a11, T a21, T a02, T a12, T a22)
{
	return a00 * det(a11, a21, a12, a22) + a01 * det(a12, a22, a10, a20) +
		   a02 * det(a10, a20, a11, a21);
}
} // namespace esu
