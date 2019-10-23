#pragma once

namespace esl::internal
{
struct Fn_assign
{
	template<class V1, class V2>
	void operator()(V1& v1, const V2& v2) const
	{
		v1 = v2;
	}
};

struct Fn_add_assign
{
	template<class V1, class V2>
	void operator()(V1& v1, const V2& v2) const
	{
		v1 += v2;
	}
};

struct Fn_sub_assign
{
	template<class V1, class V2>
	void operator()(V1& v1, const V2& v2) const
	{
		v1 -= v2;
	}
};

struct Fn_mul_assign
{
	template<class V1, class V2>
	void operator()(V1& v1, const V2& v2) const
	{
		v1 *= v2;
	}
};

struct Fn_div_assign
{
	template<class V1, class V2>
	void operator()(V1& v1, const V2& v2) const
	{
		v1 /= v2;
	}
};
} // namespace esl::internal
