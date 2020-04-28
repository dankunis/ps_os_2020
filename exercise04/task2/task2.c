#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void parent_job(int* connector);

void child_job(int* connector, char* regex);

/*
 * main first creates a named pipe, then forks a child process and
 * calls the programm flows for parent and child.
 * both get the pipe as parameter, child get's regex pattern from
 * 'argv[1]' in addition.
 * main does some basic error handling as well.
 *
 */
int main(int argc, char** argv) {
	pid_t pid;
	int connector[2];

	// check if there's an arg to be used as regex pattern in child
	if(argc < 2) {
		printf("Usage: %s <regex pattern>\n", argv[0]);
		return EXIT_FAILURE;
	}
	// initialize pipe
	if(pipe(connector) == -1) {
		printf("creation of pipe failed.\n");
		perror("pipe");
		return EXIT_FAILURE;
	}

	// fork a child
	pid = fork();

	// call correct program flow, or return an error on fork fail
	if(pid == -1) {
		printf("child process creation failed.\n");
		perror("fork");
		return EXIT_FAILURE;
	} else if(pid == 0) {
		child_job(connector, argv[1]);
	} else {
		parent_job(connector);
	}

	return EXIT_SUCCESS;
}

/*
 * program flow for parent job
 * executes 'ls' and writes result into pipe
 */
void parent_job(int* connector) {
	// connect fd of pipe to stdout and close read stream
	dup2(connector[1], STDOUT_FILENO);
	close(connector[0]);

	// call first command
	execlp("ls", "ls", NULL);
}

/*
 * program flow for child process
 * performs 'grep' on data read from pipe,
 * with regex pattern from 'argv[1]'
 */
void child_job(int* connector, char* regex) {
	// connect fd of pipe to stdin and close write stream
	dup2(connector[0], STDIN_FILENO);
	close(connector[1]);

	// call second command with regex pattern
	execlp("grep", "grep", regex, NULL);
}
