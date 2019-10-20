#include <esf/geometry/point2.hpp>

#include <iomanip>
#include <ostream>
#include <sstream>

namespace esf
{
std::string Point2::to_string() const
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << '(' << x() << ", " << y() << ')';
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Point2& p)
{
	os << p.to_string();
	return os;
}
} // namespace esf
