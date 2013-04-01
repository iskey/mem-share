cmake_minimum_required (VERSION 2.6)

#Project Name
project(mem-share)

set(CMAKE_BUILD_TYPE Debug)

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

set(TEST_SEM_SRC
	test_sem.c
	sem.c
	sem-share.c
)

set(TEST_SEM_TRG1
	sem.c
	sem-share.c
	test_sem_trg1.c
)

set(TEST_SEM_TRG2
	sem.c
	sem-share.c
	test_sem_trg2.c
)

include_directories(${PROJECT_SOURCE_DIR}/include/)

#target
add_executable(clr_mem ${CLEAR_MEM_SRC})
add_executable(test_pull ${TEST_PULL_SRC})
add_executable(test_push ${TEST_PUSH_SRC})
add_executable(test_sem ${TEST_SEM_SRC})
add_executable(test_sem_trg1 ${TEST_SEM_TRG1})
add_Executable(test_sem_trg2 ${TEST_SEM_TRG2})
