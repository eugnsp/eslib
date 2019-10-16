#pragma once
#include <cassert>

#ifdef _DEBUG
	#define ESL_VISUALIZE 1
#else
	#define ESL_VISUALIZE 0
#endif

//////////////////////////////////////////////////////////////////////////

#define ESL_USE_MKL 1

#ifndef ESL_USE_MKL
	#define ESL_USE_MKL 0
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef __GNUC__
	#define ESL_LIKELY(x) __builtin_expect(!!(x), 1)
#else
	#define ESL_LIKELY(x) (!!(x))
#endif

#ifdef NDEBUG
	#ifdef __GNUC__
		#define ESL_ASSERT(x) (void(0))
	#else
		#define ESL_ASSERT(x) (__assume(!!(x)))
	#endif
#else
	#define ESL_ASSERT(x) (ESL_LIKELY(x) ? void(0) : [] { assert(!#x); }())
#endif
