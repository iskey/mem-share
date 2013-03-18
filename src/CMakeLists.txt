cmake_minimum_required (VERSION 2.6)

#Project Name
project(mem-share)

#source list
set(TEST_PUSH_SRC
	test_push.c #
	mem-share.c
)

set(TEST_PULL_SRC
	test_pull.c #
	mem-share.c
)

set(CLEAR_MEM_SRC
	clear_mem_share.c #
	mem-share.c
)

set(CMAKE_BUILD_TYPE Debug)

#target
add_executable(clr_mem ${CLEAR_MEM_SRC})
add_executable(test_pull ${TEST_PULL_SRC})
add_executable(test_push ${TEST_PUSH_SRC})
