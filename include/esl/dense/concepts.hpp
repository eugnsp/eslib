#pragma once

#if defined(__cpp_concepts) && __GNUC__ >= 9
#define ESL_CONCEPTS
#endif

#ifdef ESL_CONCEPTS
#include <esl/dense/concepts/dense.hpp>
#endif
