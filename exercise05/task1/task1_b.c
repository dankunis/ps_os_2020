#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "common.h"

int main() {
    int shm_fd;
    void* ptr;
    int N;
    int fifo_fd;

    /* open the shared memory object */
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    /* create producer */
    pid_t child_pid;
    if((child_pid = fork()) == 0) {
        N = (int)ptr;
        for(int i=1; i<=N; i++) {
			sprintf(ptr + sizeof(int)*(i % 10 + 1), "%d", i);
		}
        exit(EXIT_SUCCESS);
    }

	/* create consumer */
    if((child_pid = fork()) == 0) {
        N = (int)ptr;
        uint64_t result = 0;
        for(int i=1; i<=N; i++) {
            result += (int)(ptr + sizeof(int)*(i % 10 + 1));
        }
        sprintf(ptr + sizeof(int)*10, "%llu", result);
        exit(EXIT_SUCCESS);
    }

    while(wait(0) > 0);

    ptr += sizeof(int) * 11;

    if ((fifo_fd = open(FIFO_NAME, O_WRONLY)) == -1) {
        error_msg("open");
    }

    // Write result to pipe
    uint64_t result = (uint64_t)ptr;
    if (write(fifo_fd, &result, sizeof(result)) < 0) {
        error_msg("write");
    }

    printf("[task1_b (%d)] Result sent to pipe\n", getpid());

    // Close and FIFO
    if (close(fifo_fd) == -1) {
        error_msg("close");
    }

    close(shm_fd);
	return EXIT_SUCCESS;
}

void error_msg(char *caller) {
    fprintf(stderr, "[task1_b (%d)] Something went wrong: ", getpid());
    perror(caller);
    exit(EXIT_FAILURE);
}
