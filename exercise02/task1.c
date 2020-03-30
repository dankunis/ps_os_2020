#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

unsigned fib(unsigned n) {
	if(n <= 1) return n;
	return fib(n - 1) + fib(n - 2);
}

int main(int argc, char* argv[]) {
	int n;

	if(argc == 2) {
		n = atoi(argv[1]);
	} else {
		printf("Specify number of child processes: ");
		scanf("%d", &n);
	}

	pid_t child_pid;

	for(int id = 0; id < n; id++) {
		if((child_pid = fork()) == 0) {
			printf("Child %d PID = %d. Fib(40) = %d\n", id, getpid(), fib(40));
			exit(0);
		}
	}

	while(wait(0) > 0)
		;
	printf("Done\n");

	return 0;
}
