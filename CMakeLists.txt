cmake_minimum_required(VERSION 2.8)

project(gingerbread)

set(BOOST_VERSION 1.37.0)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(BOOST_PACKAGES filesystem date_time program_options serialization signals thread system regex)

find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS ${BOOST_PACKAGES} REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(OpenGL)

include_directories(${Boost_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories("include")

if(MSVC)
    #
else(MSVC)
	add_definitions(
		-pipe -fsigned-char -Werror -march=native -Wno-long-long
		-static -pedantic -pedantic-errors -Wall
	)

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif()

enable_testing()

add_subdirectory(main)
add_subdirectory(gtest)
add_subdirectory(main_test)
