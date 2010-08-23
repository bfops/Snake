cmake_minimum_required(VERSION 2.8)

project(gingerbread)

add_subdirectory(Main)

add_definitions(
	-pipe -ansi -fsigned-char -fno-nonansi-builtins
	-fno-operator-names -fno-rtti -Werror -Wfatal-errors
	-Wno-long-long -pedantic -pedantic-errors -Wall -Wextra -static
)
