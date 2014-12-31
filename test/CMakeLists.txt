if (NOT BUILD_TESTING)
	return()
endif()

add_executable(xoreos-tests
	test.cpp
	common/transmatrix.cpp
	common/boundingbox.cpp
)
target_link_libraries(xoreos-tests ${xoreos_LINK_TARGETS})
add_test(all ${CMAKE_BINARY_DIR}/bin/xoreos-tests)
