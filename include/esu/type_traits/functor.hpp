#pragma once

namespace esu
{
struct Nop_fn
{
	template<class... Args>
	void operator()(const Args&...)
	{}
};
}
