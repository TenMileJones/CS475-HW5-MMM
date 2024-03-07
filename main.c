#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

// shared  globals
unsigned int mode; // 0 for S, 1 for P
unsigned int size, num_threads;
double **A, **B, **SEQ_MATRIX, **PAR_MATRIX;

int main(int argc, char *argv[]) {

	// TODO - deal with command line arguments, save the "mode"
	
	if (argc < 3){
		printf("Usage: ./mmm S <size>\nUsage: ./mmm P <num threads> <size>\n");
		exit(0);
	} 
	if ((*argv[1] != 'S') && (*argv[1] != 'P')){
		printf("Error: mode must be either S (sequential) or P (parallel)\n");
		exit(0);
	}

	
	// initialize time-storing variables
	double times[] = {0.0,0.0,0.0,0.0};
	double clockstart, clockend;

	// "size" "mode" and "num_threads" into globals so threads can see them
	if (*argv[1] == 'S') {
		mode = 0;
		num_threads = 1;
		size = atoi(argv[2]);

		// print initial summary
		printf("========\nmode: sequential\nthread count: 1\nsize: %d\n========\n", size);

		// initialize my matrices
		mmm_init();

		// warmup cache
		mmm_seq();
		mmm_reset(SEQ_MATRIX);
		
		// run 4 times
		int i = 0;
		while (i < 4){
			clockstart = rtclock();	// start the clock
			mmm_seq();
			clockend = rtclock(); // stop the clock
			// reset
			mmm_reset(SEQ_MATRIX);
			// add time to time array
			times[i] = (clockend-clockstart);
			i++;
		}

		// print time report
		double avg = (times[0]+times[1]+times[2]+times[3])/4;
		printf("Sequential Time (avg of 4 runs): %f sec\n", avg);

	} else if (*argv[1] == 'P') {
		mode = 1;
		num_threads = atoi(argv[2]);
		size = atoi(argv[3]);

		// initialize my matrices
		mmm_init();

		// warmup cache
	}

	mmm_freeup();


	// IDEAs: for assigning which rows to threads
	// do ceiling division, and then remove extra rows
	//	from last thread

	// Alternative: Evenly distrubute using floor division, and
	// then add one from remainder to each applicable thread
	// EX: 6 rows 4 threads. First: 0 1 2 3. Then: 0-1, 2-3, 4, 5.

	return 0;
}
