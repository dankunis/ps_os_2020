#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <unistd.h>

#include <semaphore.h>

#include "common.h"

/* Synchronisation with semaphore */
#define SEM_KEY  "SEM_TASK2_B"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

int main() {
    int shm_fd;
    void* ptr;
    int N;
    int fifo_fd;

    /* open the shared memory object */
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    sem_t *semaphore = sem_open(SEM_KEY, O_CREAT, SEM_PERMS, 2);
    if (semaphore == SEM_FAILED) {
        error_msg("sem_open(3) error");
    }

    /* create producer */
    pid_t child_pid;
    if((child_pid = fork()) == 0) {
        sem_t *sem = sem_open(SEM_KEY, O_RDWR);

        if (sem == SEM_FAILED) {
            error_msg("sem_open(3) child failed");
        }

        N = (int)ptr;
        for(int i=1; i<=N; i++) {
            if (sem_wait(sem) < 0) {
                error_msg("sem_wait(3) failed on child");
                continue;
            }

            /* Critical area */
			sprintf(ptr + sizeof(int)*(i % 10 + 1), "%d", i);

            if (sem_post(sem) < 0) {
                error_msg("sem_post(3) error on child");
            }
		}
        if (sem_close(sem) < 0) {
            error_msg("sem_close(3) child failed");
        }

        exit(EXIT_SUCCESS);
    }

	/* create consumer */
    if((child_pid = fork()) == 0) {
        sem_t *sem = sem_open(SEM_KEY, O_RDWR);

        if (sem == SEM_FAILED) {
            error_msg("sem_open(3) child failed");
        }

        N = (int)ptr;
        uint64_t result = 0;
        for(int i=1; i<=N; i++) {
            if (sem_wait(sem) < 0) {
                error_msg("sem_wait(3) failed on child");
                continue;
            }

            /* Critical area */
            result += (int)(ptr + sizeof(int)*(i % 10 + 1));

            if (sem_post(sem) < 0) {
                error_msg("sem_post(3) error on child");
            }
        }
        sprintf(ptr + sizeof(int)*10, "%llu", result);

        if (sem_close(sem) < 0) {
            error_msg("sem_close(3) child failed");
        }
        exit(EXIT_SUCCESS);
    }

    while(wait(0) > 0);

    ptr += sizeof(int) * 11;

    if ((fifo_fd = open(FIFO_NAME, O_WRONLY)) == -1) {
        error_msg("open");
    }

    /* Write result to pipe */
    uint64_t result = (uint64_t)ptr;
    if (write(fifo_fd, &result, sizeof(result)) < 0) {
        error_msg("write");
    }

    printf("[task1_b (%d)] Result sent to pipe\n", getpid());

    if (close(fifo_fd) == -1) {
        error_msg("close");
    }

    close(shm_fd);

    if (sem_close(semaphore) < 0) {
        sem_unlink(SEM_KEY);
        error_msg("sem_close(3) failed");
    }
    return EXIT_SUCCESS;
}

void error_msg(char *caller) {
    fprintf(stderr, "[task1_b (%d)] Something went wrong: ", getpid());
    perror(caller);
    exit(EXIT_FAILURE);
}
