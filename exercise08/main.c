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

	print_schedule(stdout, compute_schedule(data, fcfs), data);
	print_schedule(stdout, compute_schedule(data, round_robin), data);

	// -----------------------------------------------------------------------

	free_simulation_data(data);

	return EXIT_SUCCESS;
}
