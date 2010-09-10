set(TESTS
	test_cqueue
)

add_executable(Main_test ${TESTS})
target_link_libraries(Main_test
	gtest
	pthread
)
add_test(Main Main_test)