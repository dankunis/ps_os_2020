#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

pid_t child_pid;
sigset_t x;
int initialSignalSent = 0;

void sendSignalToChild()
{
    if(initialSignalSent) {
        kill(child_pid, SIGUSR1);
    } else {
        kill(child_pid, SIGUSR2);
        kill(child_pid, SIGUSR1);
        initialSignalSent = 1;
    }
    alarm(3);
}

void childTerminationHandler() {
    // check if child process doesn't exist anymore
    if(kill(child_pid, 0) != 1) {
        printf("Parent done\n");
        exit(EXIT_SUCCESS);
    }
}

void sigusr1Handler() {
    printf("Received SIGUSR1\n");
}

void sigusr2Handler() {
    printf("Child done\n");
    exit(EXIT_SUCCESS);
}

void childAlarmHandler() {
    sigprocmask(SIG_UNBLOCK, &x, NULL);
}

int main() {
    signal(SIGALRM, sendSignalToChild);

    if((child_pid = fork()) == 0) {
        // create blocker
        sigemptyset (&x);
        sigaddset(&x, SIGUSR2);
        sigprocmask(SIG_BLOCK, &x, NULL);

        signal(SIGALRM, childAlarmHandler);
        alarm(13);

        // create handlers

        struct sigaction sa1;

        sa1.sa_handler = sigusr1Handler;
        sigemptyset(&sa1.sa_mask);
        sa1.sa_flags = SA_RESTART;
        if(sigaction(SIGUSR1, &sa1, NULL) == -1) {
            printf("Error handling SIGUSR1 signal\n");
            return EXIT_FAILURE;
        }

        struct sigaction sa2;

        sa2.sa_handler = sigusr2Handler;
        sigemptyset(&sa2.sa_mask);
        sa2.sa_flags = SA_RESTART;
        if(sigaction(SIGUSR2, &sa2, NULL) == -1) {
            printf("Error handling SIGUSR2 signal\n");
            return EXIT_FAILURE;
        }

        while(1);
    }

    struct sigaction sa;

    sa.sa_handler = childTerminationHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1) {
        printf("Error handling SIGCHLD signal\n");
        return EXIT_FAILURE;
    }

    alarm(3);
    for(;;)
    {
        pause();
    }

    // not reached (use Ctrl-C to exit)
    return 0;
}
