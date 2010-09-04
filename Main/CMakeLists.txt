add_executable(GingerbreadPrototype
	Color24.cpp
	main.cpp
	Point.cpp
	Physics.cpp
	Screen.cpp
	SDL.cpp
	Snake.cpp
	Timer.cpp
)

target_link_libraries(GingerbreadPrototype
	${Boost_LIBRARIES}
	${SDL_LIBRARY}
)