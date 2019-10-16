#include <cstddef>
#include <cstdio>
#include <string>

namespace esu
{
// Returns size in bytes in a human readable form
inline std::string size_string(std::size_t size)
{
	char size_buf[100];

	if (size < 1'024UL)
		std::snprintf(size_buf, sizeof(size_buf), "%zu B", size);
	else
	{
		if (size < 1'048'576UL)
			std::snprintf(size_buf, sizeof(size_buf), "%.2f KB", size / 1'024.);
		else if (size < 1'073'741'824UL)
			std::snprintf(size_buf, sizeof(size_buf), "%.2f MB", size / 1'048'576.);
		else
			std::snprintf(size_buf, sizeof(size_buf), "%.2f GB", size / 1'073'741'824.);
	}

	return size_buf;
}
} // namespace esu
