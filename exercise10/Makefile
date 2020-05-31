CFLAGS = -std=gnu11 -pthread -Wall -Werror -Wextra -g
SOFLAGS = -fPIC -g
LDFLAGS = -pthread -ldl
SOLDFLAGS = -shared

.PHONY: all clean

all: membench free_list_global.so free_list_local.so

clean:
	$(RM) membench free_list_global.so free_list_local.so

membench: task1/membench.c
	$(CC) $(CFLAGS) $^ -o $@

free_list_global.so: task2/task2.c
	$(CC) $(SOFLAGS) $(CFLAGS) $^ -o $@ $(SOLDFLAGS)

free_list_local.so: task2/task2.c
	$(CC) $(SOFLAGS) $(CFLAGS) $^ -LOCAL_ALLOCATOR -o $@ $(SOLDFLAGS)