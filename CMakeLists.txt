cmake_minimum_required(VERSION 3.27)
project(Crossly)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.5.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(Crossly main.cpp
        core/settings.h
        core/polygon.hpp
        core/graphic_utils.hpp
        core/debug.hpp
)

target_link_libraries(Crossly sfml-graphics)