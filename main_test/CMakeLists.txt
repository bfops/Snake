set(TESTS
	test_cgq
)

add_executable(main_test ${TESTS})
target_link_libraries(main_test
	gtest
	pthread
)
add_test(main main_test)
