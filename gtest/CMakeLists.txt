set(GTEST_HEADERS
	gtest-internal-inl.h
)

set(GTEST_SRC
	gtest-death-test.cc
	gtest-filepath.cc
	gtest-port.cc
	gtest-printers.cc
	gtest-test-part.cc
	gtest-typed-test.cc
	gtest.cc
	gtest_main.cc
	# More sources here.
)

add_library(gtest ${GTEST_HEADERS} ${GTEST_SRC})
