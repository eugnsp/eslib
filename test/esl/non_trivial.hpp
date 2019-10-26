#pragma once
#include <iostream>

struct Non_trivial
{
	Non_trivial()
	{}

	Non_trivial(int i) : i(i)
	{}

	Non_trivial(const Non_trivial& nt) : i(nt.i)
	{}

	Non_trivial(Non_trivial&& nt) : i(nt.i)
	{}

	~Non_trivial()
	{}

	Non_trivial& operator=(const Non_trivial& nt)
	{
		i = nt.i;
		return *this;
	}

	Non_trivial& operator=(Non_trivial&& nt)
	{
		i = nt.i;
		return *this;
	}

	bool operator!=(Non_trivial nt) const
	{
		return i != nt.i;
	}

	bool operator==(Non_trivial nt) const
	{
		return i == nt.i;
	}

	int i = 0;
};

struct Verbose_non_trivial
{
	Verbose_non_trivial()
	{
		std::cout << "Default constructor" << std::endl;
	}

	Verbose_non_trivial(const Verbose_non_trivial&)
	{
		std::cout << "Copy constructor" << std::endl;
	}

	Verbose_non_trivial(Verbose_non_trivial&&)
	{
		std::cout << "Move constructor" << std::endl;
	}

	Verbose_non_trivial& operator=(const Verbose_non_trivial&)
	{
		std::cout << "Copy assignment" << std::endl;
		return *this;
	}

	Verbose_non_trivial& operator=(Verbose_non_trivial&&)
	{
		std::cout << "Move assignment" << std::endl;
		return *this;
	}

	~Verbose_non_trivial()
	{
		std::cout << "Destructor" << std::endl;
	}

	int data_member_;
};
