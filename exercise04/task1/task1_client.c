#include "server_config.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	if(argc != 2) {
		char error_msg[30];
		strcpy(error_msg, (argc < 2) ? "Specify clients' name" : "Illegal number of parameters");
		printf("%s\n", error_msg);
		return EXIT_FAILURE;
	}
    char fifoPath[80];
	strcpy(fifoPath, FIFO_BASE_PATH);
    strcat(fifoPath, argv[1]);

    if(access(fifoPath, F_OK ) == -1 ) {
        printf("Client is not authorized\n");
        return EXIT_FAILURE;
    }

    int fd = open(fifoPath, O_WRONLY);

//    FILE* fifoFile = fopen(fifoPath, "w+");
//	fwrite("%s connected", 1, strlen("%s connected"), fifoFile);
//	fprintf(fifoFile, , );

    while(1) {
		printf("Message:\n\n");
		char message[PIPE_BUF];

        fgets(message, PIPE_BUF, stdin);

		if(strcmp(message, "\n") == 0) {
			break;
		}
//        fwrite(message, 1, strlen(message), fifoFile);

        write(fd, message, strlen(message)+1);
    }

//	fclose(fifoFile);
    close(fd);
}

///*
// * program flow for web server
// * creates a pipe and sends a generic message every 2 - 7 seconds
// */
//void server_web(void) {
//	FILE* fp_fifo;
//	int msg;
//
//	printf("[%s] Web server launching...\n", timestamp());
//
//	/* create FIFO */
//	if(mkfifo(PATH_WEB, FIFO_MODE) == -1) {
//		perror("mkfifo");
//	}
//
//	while(1) {
//		// sleep 2 - 7 seconds
//		sleep(get_random(5, 2));
//
//		// set random message
//		msg = get_random(NUM_MSG, 0);
//
//		// open FIFO and send message
//		fp_fifo = fopen(PATH_WEB, "w+");
//		fprintf(fp_fifo, "%s [web]", messages[msg]);
//
//		fclose(fp_fifo);
//	}
//}
