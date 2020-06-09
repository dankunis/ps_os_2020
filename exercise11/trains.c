#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "trains.h"

struct train_station* create_train(int id, int arr, int stay) {
	struct train_station* a = malloc(sizeof(*a));

	a->train_id = id;
	a->arrival = arr;
	a->stay = stay;

	return a;
}

void child_process(int* platform, int num_plaforms, struct train_station* a) {
	int process_start = time(NULL);
	sem_t* semaphore = sem_open(SEM_NAME, O_RDWR);

	// wait for arrival time
	sleep(a->arrival);

	// platform occupied stores the index of the platform that is occupied
	// if occupied it is -1
	// otherwise -1
	int platform_occupied = -1;
	int boarding_start = 0;

	// while no platform available
	while(platform_occupied == -1) {
		// check if some platform is free
		// access to shared space so lock with semaphore
		if(sem_wait(semaphore) < 0) {
			perror("sem_wait(1) failed on child");
		}

		for(int i = 0; i < num_plaforms; i++) {
			if(platform[i] == 0) {
				// occupy platform, safe platform index and start boarding
				platform[i] = 1;
				platform_occupied = i;
				boarding_start = time(NULL);
				break;
			}
		}

		if(sem_post(semaphore) < 0) {
			perror("sem_post(1) error on child");
		}

		if(platform_occupied != -1) {
			// move print out of semaphore lock - less accurate on time but shorter lock time
			printf("%d seconds after start: train %d moves into the station at platform %d \n",
			       (int)difftime(time(NULL), process_start), a->train_id, platform_occupied);
		} else {
			sleep(1);
		}
	}

	// get ramining boarding time and sleep
	int remaining_boarding_time = a->stay - difftime(time(NULL), boarding_start);
	sleep(remaining_boarding_time);

	// free platform and exit
	if(sem_wait(semaphore) < 0) {
		perror("sem_wait(2) failed on child");
	}

	platform[platform_occupied] = 0;

	if(sem_post(semaphore) < 0) {
		perror("sem_post(2) error on child");
	}

	// move print out of semaphore lock - less accurate on time but shorter lock time
	printf("%d seconds after start: train %d leaves the station at platform %d \n",
	       (int)difftime(time(NULL), process_start), a->train_id, platform_occupied);

	sem_close(semaphore);

	exit(0);
}

static sem_t* open_semaphore() {
	// The following code is from homework exercise05.3
	// initialize semaphore
	sem_t* semaphore = sem_open(SEM_NAME, O_CREAT, SEM_PERMS, 1);

	// The following code is from homework exercise05.3
	if(semaphore == SEM_FAILED) {
		perror("sem_open(0) error");
		return NULL;
	}

	return semaphore;
}

int main(int argc, char** argv) {
	int num_platforms = NUM_PLATFORMS;

	// The following code is from homework exercise05.3
	// create shared memory
	int shm_fd;
	shm_fd = shm_open(SHM_KEY, O_CREAT | O_RDWR, 0600);

	// truncate file size
	off_t len = sizeof(int[num_platforms]);
	if(ftruncate(shm_fd, len) < 0) {
		perror("Truncate");
		exit(EXIT_FAILURE);
	}

	// mmap mem
	int* platforms =
	    (int*)mmap(NULL, sizeof(int[num_platforms]), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	// set all platform slots to be free
	for(int i = 0; i < num_platforms; i++) {
		platforms[i] = 0;
	}

	// create semaphore
	sem_t* semaphore = open_semaphore();

	if(semaphore == NULL) {
		close(shm_fd);
		return EXIT_FAILURE;
	}

	// create array with plane pointers
	struct train_station* train_array[NUM_PLANES] = {
		create_train(1, 0, 8),
		create_train(2, 0, 4),
		create_train(3, 2, 3),
		create_train(4, 3, 7),
		create_train(5, 5, 3),
		create_train(6, 5, 5),
        create_train(7, 6, 2)
	};

	// create child processes for each plane and execute process function
	pid_t child_pid[NUM_PLANES];
	for(int i = 0; i < 6; i++) {
		if((child_pid[i] = fork()) == 0) {
			child_process(platforms, num_platforms, train_array[i]);
			exit(EXIT_SUCCESS);
		} else if(child_pid[i] == -1) {
			perror("Error forking child process");
			exit(EXIT_FAILURE);
		}
	}

	// The following code is from homework exercise05.3
	// wait for all processes to be finished
	int status;
	int n = NUM_PLANES;
	while(n != 0) {
		wait(&status);
		n--;
	}

	// clean up sem, shm and train pointers
	sem_close(semaphore);
	close(shm_fd);
	for(int i = 0; i < NUM_PLANES; i++) {
		free(train_array[i]);
	}

	return 0;
}
