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

include_directories(${Boost_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories("include")

#TODO: release vs debug options
#e.g. SSE specs, optimization

if(MSVC)
    add_definitions(
		/Wall /WX /Y- /wd4820 /wd4121 /wd4061
		/wd4365 /wd4244 /wd4514 /wd4710 /wd4242
		/wd4267 /wd4946 /wd4625 /wd4626 /wd4371
		/wd4619 /wd4571 /wd4996 /D "MSVC"
	)
else(MSVC)
	add_definitions(
		-pipe -fsigned-char -Werror -march=native
		-pedantic -pedantic-errors -Wall
	)

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif()

#enable_testing()

add_subdirectory(main)
#add_subdirectory(gtest)
#add_subdirectory(main_test)
