cmake_minimum_required(VERSION 3.26)
project(shapereality)
set(CMAKE_CXX_STANDARD 23)

# ---------- Objective-C support -----------
# https://discourse.cmake.org/t/configure-cmake-to-only-use-objc-linker-only-on-macos-with-pch/3795/4

if (APPLE)
    enable_language(OBJC)
endif ()

# ---------- add subdirectories -------------

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(external)