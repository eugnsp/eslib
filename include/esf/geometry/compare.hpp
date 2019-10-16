#pragma once
#include <cmath>

namespace esf
{
// Maximum distance between two points so that they are considered equal
#ifdef ES_GEOM_DELTA
inline constexpr double delta = ES_GEOM_DELTA;
#else
inline constexpr double delta = 1e-6;
#endif

// The comparison x == y within predefined tolerance
struct Is_geom_equal
{
	bool operator()(double x, double y) const
	{
		return std::abs(x - y) <= delta;
	}
};

// The comparison x < y within predefined tolerance
struct Is_geom_less
{
	bool operator()(double x, double y) const
	{
		return x < y - delta;
	}
};

// The comparison x > y within predefined tolerance
struct Is_geom_greater
{
	bool operator()(double x, double y)
	{
		return Is_geom_less{}(y, x);
	}
};

// The comparison x <= y within predefined tolerance
struct Is_geom_less_equal
{
	bool operator()(double x, double y)
	{
		return !Is_geom_greater{}(x, y);
	}
};

// The comparison x >= y within predefined tolerance
struct Is_geom_greater_equal
{
	bool operator()(double x, double y)
	{
		return Is_geom_less_equal{}(y, x);
	}
};

inline bool is_geom_equal(double x, double y)
{
	return Is_geom_equal{}(x, y);
}

inline bool is_geom_less(double x, double y)
{
	return Is_geom_less{}(x, y);
}

inline bool is_geom_greater(double x, double y)
{
	return Is_geom_greater{}(x, y);
}

inline bool is_geom_less_equal(double x, double y)
{
	return Is_geom_less_equal{}(x, y);
}

inline bool is_geom_greater_equal(double x, double y)
{
	return Is_geom_greater_equal{}(x, y);
}
} // namespace esf
