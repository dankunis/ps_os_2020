/**
 * Creates a shared memory segment and opens a named FIFO and listens
 * for a message.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char** argv)
{
	int N;
    if(argc != 2) {
        printf("Usage: %s <N - number to be written into shared memory>\n", argv[0]);
        return EXIT_FAILURE;
    } else {
		N = atoi(argv[1]);
	}

    int shm_fd;
    void* ptr;
    int fifo_fd;
	uint64_t result;

    /* create and configure the shared memory object */
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sprintf(ptr, "%d", N);

    /* IPC: named pipe */
    // create named FIFO
    if (mkfifo(FIFO_NAME, S_IRWXU) == -1) {
        error_msg("mkfifo");
    }

    // open for reading
    if ((fifo_fd = open(FIFO_NAME, O_RDONLY)) == -1) {
        error_msg("open");
    }
    // read from FIFO
    if (read(fifo_fd, &result, sizeof(uint64_t)) == -1) {
        error_msg("read");
    }

    printf("Result: %llu\n", result);

    // Close FIFO
    if (unlink(FIFO_NAME) == -1) {
        error_msg("unlink");
    }
    if (close(fifo_fd) == -1) {
        error_msg("close");
    }

    return EXIT_SUCCESS;
}

/*
 * Prints error message to stdout and terminates
 */
void error_msg(char *caller) {
    fprintf(stderr, "[task1_a (%d)] Something went wrong: ", getpid());
    perror(caller);
    exit(EXIT_FAILURE);
}
