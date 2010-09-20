add_executable(gingerbread-in-c
	graphics.c
	main.c
)

target_link_libraries(gingerbread-in-c
	${SDL_LIBRARY}
	${OPENGL_LIBRARY}
)