add_executable(GingerbreadPrototype
	Color24.cpp
	main.cpp
	Point.cpp
	Screen.cpp
	Snake.cpp
)

target_link_libraries(GingerbreadPrototype
	${Boost_LIBRARIES}
	${SDL_LIBRARY}
)