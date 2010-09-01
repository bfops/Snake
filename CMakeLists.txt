cmake_minimum_required(VERSION 2.8)

project(gingerbread)

set(BOOST_VERSION 1.37.0)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(BOOST_PACKAGES filesystem date_time program_options serialization signals thread system regex)

find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS ${BOOST_PACKAGES})
find_package(SDL)

include_directories(${Boost_INCLUDE_DIR} ${SDL_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})# ${SDL_INCLUDE_DIR})

add_definitions(
	-pipe -ansi -fsigned-char -fno-nonansi-builtins
	-fno-operator-names -fno-rtti -Werror -Wfatal-errors
	-Wno-long-long -static -pedantic -pedantic-errors -Wall #-Wextra
)

add_subdirectory(Main)
