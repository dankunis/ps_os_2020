cmake_minimum_required(VERSION 3.16)
project(tasks C)

set(CMAKE_C_STANDARD 11)

include_directories(example_makefile)
include_directories(exercise02)
include_directories(exercise04)
include_directories(exercise04/task1)
include_directories(exercise04/task2)
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
    self_assessment_test/bstree.c
    self_assessment_test/bstree.h
    self_assessment_test/bstree_test.c)