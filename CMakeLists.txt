cmake_minimum_required(VERSION 3.13)
project(eslib VERSION 1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE CPP_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(eslib "${CPP_FILES}")
target_include_directories(eslib PUBLIC "include")
target_compile_features(eslib PUBLIC cxx_std_17)
target_compile_options(eslib PRIVATE -Wall -Wpedantic -Wextra -Werror=return-type -m64 -march=native 
					   $<$<CONFIG:DEBUG>:-O0 -g> $<$<CONFIG:RELEASE>:-Wno-unused-parameter -Wno-deprecated-declarations -O3 -DNDEBUG>)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(eslib PRIVATE -Wno-psabi)
endif()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
target_link_libraries(eslib PUBLIC Threads::Threads)

target_include_directories(eslib PUBLIC "$ENV{MKLROOT}/include")
target_compile_definitions(eslib PUBLIC MKL_ILP64)
set(MKL_LIB "$ENV{MKLROOT}/lib/intel64")
set(MKL_LIBRARIES "-Wl,--no-as-needed" mkl_intel_ilp64 mkl_sequential mkl_core Threads::Threads m dl)

target_link_directories(eslib PUBLIC "${MKL_LIB}")
target_link_libraries(eslib PUBLIC "${MKL_LIBRARIES}")

find_package(Boost)
#enable_testing()
#add_executable(eslib_test "test/esl_test.cpp")
#if (Boost_FOUND)
#	target_include_directories(esl_test PUBLIC "${Boost_INCLUDE_DIRS}")
#endif()
#target_link_libraries(esl_test PUBLIC esl)
#target_compile_options(esl_test PRIVATE -Wall -Wpedantic -Wextra -Werror=return-type -m64 -march=native -O0 -g)

#if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#	target_compile_options(esl PRIVATE -Wno-psabi)
#	target_compile_options(esl_test PRIVATE -Wno-psabi)
#endif()

#add_test(NAME esl_test COMMAND esl_test)

