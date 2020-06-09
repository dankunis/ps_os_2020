#ifndef TASKS_TRAINS_H
#define TASKS_TRAINS_H

#define SHM_KEY "SHM_TRAIN_STATION"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define SEM_NAME "SEM_TRAIN_STATIONT"

#define NUM_PLANES 7
#define NUM_PLATFORMS 3

struct train_station {
    int train_id;
    int arrival;
    int stay;
};

struct train_station* create_train(int id, int arr, int stay);

static void child_process(int * platform, int num_plaforms, struct train_station*a);

#endif // TASKS_TRAINS_H
