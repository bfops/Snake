add_executable(GingerbreadPrototype
	main.cpp
	Snake.cpp
)

target_link_libraries(GingerbreadPrototype
	${Boost_LIBRARIES}
	${pthread}
	${SDL_LIBRARY}
)