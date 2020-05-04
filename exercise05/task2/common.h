#ifndef TASKS_COMMON_H
#define TASKS_COMMON_H

/* Preferences IPC */
#define SHM_NAME "SHM_OS"
#define SHM_SIZE sizeof(long)

#define FIFO_NAME "FIFO_WITH_RESULT"

/* Error handler */
void error_msg(char *caller);

#endif // TASKS_COMMON_H
