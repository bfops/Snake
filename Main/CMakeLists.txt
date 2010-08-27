add_executable(GingerbreadPrototype
	main.cpp
)

SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lpthread")

target_link_libraries(GingerbreadPrototype
	${Boost_LIBRARIES}
)