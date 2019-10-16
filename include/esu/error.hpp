#pragma once
#include <string>
#include <sstream>

namespace esu
{
// Error reporting class
class Error
{
public:
	Error() = default;

	// Checks if error occured
	operator bool() const
	{
		return error_;
	}

	// Appends values to the error message and sets the error flag
	template<typename... Ts>
	void append(const Ts&... values)
	{
		(message_ << ... << values);
		error_ = true;
	}

	// Appends values to the error message followed by a line break and sets the error flag
	template<typename... Ts>
	void append_ln(const Ts&... values)
	{
		append(values..., '\n');
	}

	// Returns the error message
	std::string message() const
	{
		return message_.str();
	}

	// Resets the error flag
	void reset_error_flag()
	{
		error_ = false;
	}

	// Resets the error flag and clears the error message
	void clear()
	{
		reset_error_flag();
		message_.str("");
	}

private:
	bool error_ = false;
	std::stringstream message_;
};
} // namespace esu
