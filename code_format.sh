#!/bin/sh

find ./include -type f \( -iname "*.hpp" -or -iname "*.cpp" \) | xargs clang-format -style=file -i
find ./src -type f \( -iname "*.hpp" -or -iname "*.cpp" \) | xargs clang-format -style=file -i
