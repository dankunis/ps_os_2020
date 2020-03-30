#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

pid_t child_pid;

static void handler(int signum) {
	if(signum != SIGCHLD) {
		return;
	}

	// check if child process doesn't exist anymore
	if(kill(child_pid, 0) != 1) {
        printf("Parent done\n");
        exit(EXIT_SUCCESS);
    }
}

int main() {
	struct sigaction sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD, &sa, NULL) == -1) {
		printf("Error handling SIGCHLD signal\n");
		return EXIT_FAILURE;
	}

	if((child_pid = fork()) == 0) {
		printf("Child %d sleeping for 5 seconds...\n", getpid());
		sleep(5);
		printf("Child done\n");
		exit(0);
	}

	while(1);

	return EXIT_SUCCESS;
}