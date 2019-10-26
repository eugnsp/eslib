#pragma once
#include <es/la/dense.hpp>
#include <string>
#include <iostream>
#include <initializer_list>
#include <typeinfo>

#define REQUIRE(x) (void)((!!(x)) || (require((unsigned int)(__LINE__), #x), 0))
#define REQUIRE_STATIC(x) static_assert((x), "REQUIRE_STATIC failed: "#x)

void 
#ifdef _MSC_VER
__declspec(noinline)
#else
__attribute__ ((noinline))
#endif
require(int line, const char* str)
{
	std::cout << "REQUIRE failed at line " << line << '\n' << str << '\n';
	std::cout << "Aborted\n";
	std::exit(-1);
}

template<typename T>
class Test_base
{
public:
	void run()
	{
		std::cout << name() << " <T = " << typeid(T).name() << ">... ";
		run_test();
		std::cout << "Passed!" << std::endl;
	}

	virtual std::string name() const = 0;

protected:
	virtual void run_test() = 0;
};

template<class E>
bool matrix_elements(const E& expr, typename E::Value value)
{
	for (std::size_t col = 0; col < expr.cols(); ++col)
		for (std::size_t row = 0; row < expr.rows(); ++row)
			if (expr(row, col) != value)
				return false;

	return true;
}

template<class E>
bool matrix_elements(const E& expr, std::initializer_list<typename E::Value> values)
{
	if (expr.size() != values.size())
		return false;

	auto value = values.begin();
	for (std::size_t col = 0; col < expr.cols(); ++col)
		for (std::size_t row = 0; row < expr.rows(); ++row)
			if (expr(row, col) != *value++)
			return false;

	return true;
}