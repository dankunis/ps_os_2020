/**
 * According to MAN:
 * "The thread can be canceled at any time.  (Typically, it will
 * be canceled immediately upon receiving a cancellation request,
 * but the system doesn't guarantee this.)"
 * So, even if the thread was cancelled -
 * the corresponding file may not have been deleted
 */

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int random_gen(int min, int max);
void thread_cleanup(void* args);

void* thread_work_load(void* args) {
	// enable asynchronous cancelling only after file is created
	if(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0) {
		perror("pthread_setcancelstate to PTHREAD_CANCEL_DISABLE");
		exit(EXIT_FAILURE);
	}

	char file_name[12];

	snprintf(file_name, sizeof(char) * 12, "thread%d.txt", (int)(intptr_t)args);
	fopen(file_name, "w");

	pthread_cleanup_push(&thread_cleanup, (void*)file_name);

	if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0) {
		perror("pthread_setcancelstate to PTHREAD_CANCEL_ENABLE");
		exit(EXIT_FAILURE);
	}
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0) {
		perror("pthread_setcanceltype");
		exit(EXIT_FAILURE);
	}

	sleep(2);

	// pop cleaner function without execution
	pthread_cleanup_pop(0);
	return NULL;
}

int main() {
	srand(time(NULL));

	pthread_t t[10];
	for(int i = 0; i < 10; i++) {
		if(pthread_create(&t[i], NULL, thread_work_load, (void*)(intptr_t)i) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	sleep(1);

	for(int i = 0; i < 10; i++) {
		int kill_thread = random_gen(0, 1);
		if(kill_thread) {
			if(pthread_cancel(t[i]) != 0) {
				perror("pthread_cancel");
				exit(EXIT_FAILURE);
			}
			if(pthread_detach(t[i]) != 0) {
				perror("pthread_detach");
				exit(EXIT_FAILURE);
			}
			printf("Thread %d cancelled\n", i);
		} else {
			if(pthread_join(t[i], NULL) != 0) {
				perror("pthread_join");
				exit(EXIT_FAILURE);
			}
			printf("Thread %d not cancelled\n", i);
		}
	}

	return EXIT_SUCCESS;
}

void thread_cleanup(void* args) {
	if(args) {
		if(remove((char*)args) != 0) {
			perror("remove");
			exit(EXIT_FAILURE);
		}
	} else {
		perror("No args for cleanup");
		exit(EXIT_FAILURE);
	}
}

int random_gen(int min, int max) {
	return rand() % (max - min + 1) + min;
}
