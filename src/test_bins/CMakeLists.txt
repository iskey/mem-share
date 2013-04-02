cmake_minimum_required (VERSION 2.6)

#source list
set(TEST_PUSH_SRC
	test_push.c #
	${PROJECT_SOURCE_DIR}/src/mem-share.c
)

set(TEST_PULL_SRC
	test_pull.c #
	${PROJECT_SOURCE_DIR}/src/mem-share.c
)

set(CLEAR_MEM_SRC
	clear_mem_share.c #
	${PROJECT_SOURCE_DIR}/src/mem-share.c
)

set(TEST_SEM_SRC
	test_sem.c
	${PROJECT_SOURCE_DIR}/src/base-sem.c
	${PROJECT_SOURCE_DIR}/src/sem-trigger.c
)

set(TEST_SEM_TRG1
	${PROJECT_SOURCE_DIR}/src/base-sem.c
	${PROJECT_SOURCE_DIR}/src/sem-trigger.c
	test_sem_trg1.c
)

set(TEST_SEM_TRG2
	${PROJECT_SOURCE_DIR}/src/base-sem.c
	${PROJECT_SOURCE_DIR}/src/sem-trigger.c
	test_sem_trg2.c
)

#target
add_executable(clr_mem ${CLEAR_MEM_SRC})
add_executable(test_pull ${TEST_PULL_SRC})
add_executable(test_push ${TEST_PUSH_SRC})
add_executable(test_sem ${TEST_SEM_SRC})
add_executable(test_sem_trg1 ${TEST_SEM_TRG1})
add_Executable(test_sem_trg2 ${TEST_SEM_TRG2})
