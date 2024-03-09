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
	
	// basic error handling
	if (argc < 3){
		printf("Usage: ./mmm S <size>\nUsage: ./mmm P <num threads> <size>\n");
		exit(0);
	} 
	if ((*argv[1] != 'S') && (*argv[1] != 'P')){
		printf("Error: mode must be either S (sequential) or P (parallel)\n");
		exit(0);
	}

	// initialize time-storing variables
	double sequentialTimes[] = {0.0,0.0,0.0,0.0};
	double clockstart, clockend;

	// put "size" "mode" and "num_threads" into globals so threads can see them
	if (*argv[1] == 'S') {
		mode = 0;
		num_threads = 1;
		size = atoi(argv[2]);

		// print initial summary
		printf("========\nmode: sequential\nthread count: %d\nsize: %d\n========\n", num_threads, size);

		// initialize my matrices
		mmm_init();

		// warmup cache
		mmm_seq();
		mmm_reset(SEQ_MATRIX);
		
		// run 4 times
		for (int i = 0; i < 4; i++){
			clockstart = rtclock();	// start the clock
			mmm_seq();
			clockend = rtclock(); // stop the clock
			// reset
			mmm_reset(SEQ_MATRIX);
			// add time to time array
			sequentialTimes[i] = (clockend-clockstart);
		}

		// print time report
		double avg = (sequentialTimes[0]+sequentialTimes[1]+sequentialTimes[2]+sequentialTimes[3])/4;
		printf("Sequential Time (avg of 4 runs): %f sec\n", avg);

	} else if (*argv[1] == 'P') {

		// please forgive the atrocious amount of code duplication,
		//  it's the day before spring break and I am tired

		double parallelTimes[] = {0.0,0.0,0.0,0.0};
		mode = 1;
		num_threads = atoi(argv[2]);
		size = atoi(argv[3]);

		// print initial summary
		printf("========\nmode: parallel\nthread count: %d\nsize: %d\n========\n", num_threads, size);

		// initialize my matrices
		mmm_init();

		if (num_threads == 0) {
			printf("Really? 0 threads? I guess we're waiting all day...\n");
			mmm_freeup();
			exit(0);
		}
		
		// warmup cache for sequential
		mmm_seq();
		mmm_reset(SEQ_MATRIX);

		// run sequential 4 times for comparison
		for (int i = 0; i < 4; i++){
			clockstart = rtclock();	// start the clock
			mmm_seq();
			clockend = rtclock(); // stop the clock
			// reset
			mmm_reset(SEQ_MATRIX);
			// add time to time array
			sequentialTimes[i] = (clockend-clockstart);
		}

		// warmup cache for parallel
		// calculate how many rows to give each thread
		int rowsPerThread = size/num_threads;
		int extraRows = size%num_threads;

		// amount to offset start and end row based on distribution of extra rows between threads.
		// Alternative: number of extra rows already assigned to previous threads.
		int offset = 0;

		// assign thread arguments
		thread_args *args = (thread_args*) malloc(num_threads * sizeof(thread_args));
		for (int i = 0; i < num_threads; i++) {
			args[i].tid = i;
			args[i].start = offset+(i*rowsPerThread);
			args[i].end = offset+((i+1)*rowsPerThread);

			// decide whether this thread receives an extra row
			if (i < extraRows) {
				// give this thread an extra row
				offset++;
				args[i].end++;
			}
		}

		// allocate space to hold threads
		pthread_t *threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));

		// create threads
		for (int i = 0; i < num_threads; i++) {
			pthread_create(&threads[i], NULL, mmm_par, &args[i]);
		}

		// wait for threads to finish
		for (int i = 0; i < num_threads; i++) {
			pthread_join(threads[i], NULL);
		}
		
		// reset matrix and free allocated memory
		mmm_reset(PAR_MATRIX);
		free(threads);
		free(args);
		threads = NULL;
		args = NULL;

		// run parallel 4 times
		for (int i = 0; i < 4; i++){
			clockstart = rtclock();	// start the clock
			
			// calculate how many rows to give each thread
			int rowsPerThread = size/num_threads;
			int extraRows = size%num_threads;

			// amount to offset start and end row based on distribution of extra rows between threads.
			// Alternative: number of extra rows already assigned to previous threads.
			int offset = 0;

			// assign thread arguments
			thread_args *args = (thread_args*) malloc(num_threads * sizeof(thread_args));
			for (int i = 0; i < num_threads; i++) {
				args[i].tid = i;
				args[i].start = offset+(i*rowsPerThread);
				args[i].end = offset+((i+1)*rowsPerThread);

				// decide whether this thread receives an extra row
				if (i < extraRows) {
					// give this thread an extra row
					offset++;
					args[i].end++;
				}
			}

			// allocate space to hold threads
			pthread_t *threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));

			// create threads
			for (int i = 0; i < num_threads; i++) {
				pthread_create(&threads[i], NULL, mmm_par, &args[i]);
			}

			// wait for threads to finish
			for (int i = 0; i < num_threads; i++) {
				pthread_join(threads[i], NULL);
			}

			clockend = rtclock(); // stop the clock
			// reset and free
			mmm_reset(PAR_MATRIX);
			free(threads);
			free(args);
			threads = NULL;
			args = NULL;
			
			// add time to time array
			parallelTimes[i] = (clockend-clockstart);
		}
		
		// print summary
		double sequentialAvg = (sequentialTimes[0]+sequentialTimes[1]+sequentialTimes[2]+sequentialTimes[3])/4;
		printf("Sequential Time (avg of 4 runs): %f sec\n", sequentialAvg);
		double parallelAvg = (parallelTimes[0]+parallelTimes[1]+parallelTimes[2]+parallelTimes[3])/4;
		printf("Parallel Time (avg of 4 runs): %f sec\n", parallelAvg);
		printf("Speedup: %f\n", sequentialAvg/parallelAvg);
		printf("Verifying... ");
		printf("largest error between parallel and sequential matrix: %f\n", mmm_verify());
	}

	mmm_freeup();

	return 0;
}
