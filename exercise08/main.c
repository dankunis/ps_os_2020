#include <math.h>
#include <stdio.h>

#include "scheduling_sim.h"
#include "scheduling_utility.h"

// ---------------------------------------------------------------------------
// Example schedulers
// ---------------------------------------------------------------------------

process_t* fcfs(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);
	return selected;
}

process_t* round_robin(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);
	// We are allowed to delete the scheduled process from the list.
	// It will be re-appended at the end (after any newly arrived processes).
	my_array_delete(processes, 0);
	return selected;
}

// ---------------------------------------------------------------------------
// Implement your schedulers here
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Task 1
// ---------------------------------------------------------------------------

process_t* srt(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);

	for(size_t i = 1; i < my_array_size(processes); ++i) {
		process_t* current_process = my_array_get(processes, i);
		if(current_process->remaining_time < selected->remaining_time) {
			selected = current_process;
		}
	}

	return selected;
}

process_t* round_robin_q4(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);

	selected->user1++;

	if(selected->user1 == 4) {
		selected->user1 = 0;
		my_array_delete(processes, 0);
	}

	return selected;
}

// ---------------------------------------------------------------------------
// Task 2
// ---------------------------------------------------------------------------

process_t* hpf_preemptive(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);

	for(size_t i = 1; i < my_array_size(processes); ++i) {
		process_t* current_process = my_array_get(processes, i);
		if(current_process->priority > selected->priority) {
			selected = current_process;
		}
	}

	return selected;
}

process_t* hpf_non_preemptive(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);

	for(size_t i = 1; i < my_array_size(processes); ++i) {
		process_t* current_process = my_array_get(processes, i);
		if(selected->user1 != 1 && current_process->priority > selected->priority) {
			selected = current_process;
		}
	}

	selected->user1 = 1;

	return selected;
}

// ---------------------------------------------------------------------------
// Task 3
// ---------------------------------------------------------------------------

struct process_ticket {
	char name;
	int num_tickets;
	int* tickets;
};

void clean_compensation(my_array * processes) {
    for (size_t i = 0; i < my_array_size(processes); i++) {
		process_t* proc = my_array_get(processes, i);
		proc->user3 = 0;
	}
}

process_t* get_process_by_name (my_array * processes, char name) {
    for (size_t i = 0; i < my_array_size(processes); i++) {
        process_t* proc = my_array_get(processes, i);

        if (proc -> name == name) {
            return proc;
        }
    }

    return NULL;
}

process_t* find_and_compensate_working_p(my_array* processes, int quantum) {
	for(size_t i = 0; i < my_array_size(processes); ++i) {
		process_t* proc = my_array_get(processes, i);
		if(proc->user1 == 1) {
			int yielded = scheduler_rand(1, 1000) % 2;
			if(yielded) {
				proc->user1 = 0;

				// give compensation
				if(proc->user2 < quantum) {
					proc->user3 = 1 / (proc->user2);
				}

				proc->user2 = 0;
				break;
			}

			if(proc->user2 == quantum) {
				proc->user1 = 0;
				proc->user2 = 0;
				proc->user3 = 0;
				break;
			}

			// process continues work
			return proc;
		}
	}

	return NULL;
}

process_t* lottery(int timestep, my_array* processes) {
	process_t* selected = NULL;
	int quantum = 3;

	// find working process
	selected = find_and_compensate_working_p(processes, quantum);
	if(selected != NULL) return selected;

	// create tickets for each process
	struct process_ticket* pts[my_array_size(processes)];
	for(size_t i = 0; i < my_array_size(processes); i++) {
		process_t* proc = my_array_get(processes, i);

		int compensation = proc->user3 > 0 ? proc->user3 : 1;
		int num_tickets = roundf((30 / proc->service_time) * compensation);

		struct process_ticket* pt = malloc(sizeof(*pt));
		pt->tickets = malloc(sizeof(int*) * num_tickets);
		pt->name = proc->name;
		pt->num_tickets = num_tickets;

		for(int r = 0; r <= num_tickets; r++) {
			pt->tickets[r] = scheduler_rand(0, 30);
		}

		pts[i] = pt;
	}

    //scheduler choose ticket
    int rand_ticket = scheduler_rand(0, 30);

    // find first process with ticket
    char process_name;
    for (size_t i = 0; i < my_array_size(processes); i++) {
        struct process_ticket *pt = pts[i];

        for (int r = 0; r < pt -> num_tickets; r++) {
            if (pt -> tickets[r] == rand_ticket) {
                process_name = pt -> name;
                break;
            }
        }
    }

    selected = get_process_by_name(processes, process_name);
    clean_compensation(processes);

    if(selected == NULL) { selected = my_array_get(processes, 0); }
    selected-> user1 = 1;
    selected -> user2 += 1;


    // clean tickets
    for (size_t i = 0; i < my_array_size(processes); i++) {
        struct process_ticket *pt = pts[i];
        free(pt);
    }

    return selected;
}

// ---------------------------------------------------------------------------

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "Error: Usage: %s <filename>\nExample: %s input.csv\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	simulation_data_t* data = read_simulation_data(argv[1]);
	if(!data) {
		fprintf(stderr, "Error: Unable to open the file '%s' for reading!\n", argv[1]);
		return EXIT_FAILURE;
	}
	print_simulation_data(stdout, data);

	// -----------------------------------------------------------------------
	// Insert calls to your schedulers here
	// -----------------------------------------------------------------------

	print_schedule(stdout, compute_schedule(data, srt), data);
	print_schedule(stdout, compute_schedule(data, round_robin), data);
	print_schedule(stdout, compute_schedule(data, round_robin_q4), data);
	print_schedule(stdout, compute_schedule(data, hpf_preemptive), data);
	print_schedule(stdout, compute_schedule(data, hpf_non_preemptive), data);
    print_schedule(stdout, compute_schedule(data, lottery), data);

	// -----------------------------------------------------------------------

	free_simulation_data(data);

	return EXIT_SUCCESS;
}
