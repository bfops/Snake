add_executable(gingerbread-in-c
	event.c
	graphics.c
	lookup_tables.c
	main.c
	math.c
)

target_link_libraries(gingerbread-in-c
	${SDL_LIBRARY}
	${OPENGL_LIBRARY}
)

