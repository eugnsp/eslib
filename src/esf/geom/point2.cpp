#include <esf/geometry/point2.hpp>

#include <iomanip>
#include <ostream>
#include <sstream>

namespace esf
{
std::ostream& operator<<(std::ostream& os, const Point2& p)
{
	std::stringstream ss;

	ss << std::fixed << std::setprecision(2) << '(' << p.x() << ", " << p.y() << ')';
	os << ss.str() << std::flush;

	return os;
}
} // namespace esf
