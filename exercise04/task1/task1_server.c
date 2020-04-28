#include "server_config.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

struct ServerClient {
	char name[30];
	int fd;
	char fifoPath[80];
};

typedef struct ServerClient ServerClient;

void log_message(int fd) {
    char buf[100];
    read(fd, buf, sizeof(buf));
    printf("%s\n", buf);
    fflush(stdout);

    // clear buffer
    memset(buf, 0, sizeof(buf));
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("Server won't start with no clients to connect");
		return EXIT_SUCCESS;
	}

	fd_set fs;
	FD_ZERO(&fs);
	int maxFD = -1;

	ServerClient serverClients[argc - 1];

	for(int i = 1; i < argc; i++) {
		ServerClient newClient;

		strcpy(newClient.name, argv[i]);
		strcpy(newClient.fifoPath, FIFO_BASE_PATH);
		strcat(newClient.fifoPath, argv[i]);

		if(mkfifo(newClient.fifoPath, 0644)) {
			perror("mkfifo");
		}

        newClient.fd = open(newClient.fifoPath, O_RDONLY);
		FD_SET(newClient.fd, &fs);

		if (newClient.fd > maxFD) {
			maxFD = newClient.fd;
		}

		serverClients[i] = newClient;

		log_message(newClient.fd);
	}

	int connectedClients = argc;

	while (connectedClients > 0) {
        select(maxFD+1, &fs, NULL, NULL, NULL);

        for(int i = 0; i < argc; i++) {
            if(FD_ISSET(serverClients[i].fd, &fs)) {
				log_message(serverClients[i].fd);
				break;
			}
        }
		break;
	}

	for(int i = 0; i < argc; i++) {
		close(serverClients[i].fd);
		remove(serverClients[i].fifoPath);
	}

	return EXIT_SUCCESS;
}

/*
 * program flow for logging server
 * opens all FIFOs and listens for messages.
 */
//void server_log(void) {
//	sleep(1);
//	// file pointers for FIFOs
//	FILE* fp_fifo_db;
//	FILE* fp_fifo_mw;
//	FILE* fp_fifo_web;
//
//	// file descriptor for each pipe
//	int fd_db;
//	int fd_mw;
//	int fd_web;
//	int fd_next;
//
//	// file descriptor set
//	fd_set fs;
//	FD_ZERO(&fs);
//
//	// open FIFOs
//	fp_fifo_db = fopen(PATH_DB, "r+");
//	fp_fifo_mw = fopen(PATH_MW, "r+");
//	fp_fifo_web = fopen(PATH_WEB, "r+");
//
//	// check for correct initialization of pipes
//	if(fp_fifo_db == NULL || fp_fifo_mw == NULL || fp_fifo_web == NULL) {
//		printf("error occured while opening FIFOs.\n");
//		exit(EXIT_FAILURE);
//	}
//
//	/* loop while listening for messages */
//	while(1) {
//		// set file descriptors
//		fd_db = fileno(fp_fifo_db);
//		fd_mw = fileno(fp_fifo_mw);
//		fd_web = fileno(fp_fifo_web);
//
//		// set highest file descriptor and increment
//		fd_next = get_max(fd_mw, fd_web, fd_db);
//		fd_next++;
//
//		// clear file desciptor set
//		FD_ZERO(&fs);
//		// add FIFOs file descriptors
//		FD_SET(fd_db, &fs);
//		FD_SET(fd_mw, &fs);
//		FD_SET(fd_web, &fs);
//
//		// wait for message
//		select(fd_next, &fs, NULL, NULL, NULL);
//
//		/*
//		 * print message from filled pipe
//		 */
//		if(FD_ISSET(fd_db, &fs)) {
//			log_message(fd_db);
//		}
//
//		if(FD_ISSET(fd_mw, &fs)) {
//			log_message(fd_mw);
//		}
//
//		if(FD_ISSET(fd_web, &fs)) {
//			log_message(fd_web);
//		}
//	}
//}
