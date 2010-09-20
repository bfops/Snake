add_executable(gingerbread-in-c
	main.c
	screen.c
)

target_link_libraries(gingerbread-in-c
	${SDL_LIBRARY}
)