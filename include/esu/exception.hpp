#pragma once
#include <cstddef>
#include <stdexcept>
#include <string>

namespace esu
{
class UnreachableException : public std::logic_error
{
public:
	UnreachableException(const std::string& function, const std::string& file, std::size_t line) :
		logic_error(
			"Unreachable exception in " + function + " (" + file + ':' + std::to_string(line) + ')')
	{}
};

class RuntimeException : public std::runtime_error
{
public:
	RuntimeException(const std::string& error) : runtime_error(error)
	{}

	RuntimeException(
		const std::string& error,
		const std::string& function,
		const std::string& file,
		std::size_t line) :
		runtime_error(error + " in " + function + " (" + file + ':' + std::to_string(line) + ')')
	{}
};

class Not_implemented_exception : public std::logic_error
{
public:
	Not_implemented_exception() : logic_error("Unimplemented exception")
	{}

	Not_implemented_exception(const std::string& error) :
		logic_error("Unimplemented exception in " + error)
	{}
};

class FileReadOpenException : public std::runtime_error
{
public:
	FileReadOpenException(const std::string& name) :
		runtime_error("File " + name + " cannot be opened for reading")
	{}
};

class FileWriteException : public std::runtime_error
{
public:
	FileWriteException(const std::string& name) :
		runtime_error("File " + name + " cannot be written")
	{}
};

class BadFileException : public std::runtime_error
{
public:
	BadFileException(const std::string& fileName) :
		runtime_error("File " + fileName + " is corrupted")
	{}
};
} // namespace esu
