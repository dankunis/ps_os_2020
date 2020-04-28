CFLAGS = -std=gnu11 -Wall -Werror -Wextra -O2

.PHONY: all clean

all: task1 task2

clean: $(RM) task1/task1 task2/task2

task1: task1/task1_server.c task1/task1_server task1/task1_client.c task1/task1_client

task2: task2/task2.c task2/task2
