cmake_minimum_required(VERSION 3.16)
project(tasks C)

set(CMAKE_C_STANDARD 11)

include_directories(example_makefile)
include_directories(exercise02)
include_directories(exercise04)
include_directories(exercise04/task1)
include_directories(exercise04/task2)
include_directories(exercise05)
include_directories(exercise05/task1)
include_directories(exercise05/task2)
include_directories(exercise06)
include_directories(exercise07)
include_directories(exercise08)
include_directories(exercise09)
include_directories(self_assessment_test)

add_executable(tasks
    example_makefile/common.c
    example_makefile/common.h
    example_makefile/program1.c
    example_makefile/program2.c
    exercise02/task1.c
    exercise02/task2.c
    exercise02/task3.c
    exercise04/task1/server_config.h
    exercise04/task1/task1_client.c
    exercise04/task1/task1_server.c
    exercise04/task2/task2.c
    exercise05/task1/task1_a.c
    exercise05/task1/task1_b.c
    exercise05/task2/task2_a.c
    exercise05/task2/task2_b.c
    exercise06/task2/myqueue.h
    exercise06/task2/task2.c
    exercise06/task3/philosophers.c
    exercise06/task1/task1.c
    exercise07/myqueue.h
    exercise07/task1/task1.c
    exercise07/task2/task2.c
    exercise07/task3/task3_mutex.c
    exercise07/task3/task3_atomic.c
    exercise08/main.c
    exercise08/my_array.c
    exercise08/my_array.h
    exercise08/scheduling_sim.c
    exercise08/scheduling_sim.h
    exercise08/scheduling_utility.c
    exercise08/scheduling_utility.h
    self_assessment_test/bstree.c
    self_assessment_test/bstree.h
    self_assessment_test/bstree_test.c
    exercise09/common/membench.c
    exercise09/common/allocator.h
    exercise09/task1/system_allocator.c
    exercise09/task2_3/task2_3.c
    exercise10/task1/membench.c
    exercise10/task1/allocator.h
    exercise10/task2/task2.c
    exercise10/task2/free_list_allocator.h
    exercise11/trains.c
    exercise11/trains.h)

add_executable(train
        exercise11/trains.c
        exercise11/trains.h)
