#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

// shared  globals
unsigned int mode;
unsigned int size, num_threads;
double **A, **B, **SEQ_MATRIX, **PAR_MATRIX;

int main(int argc, char *argv[]) {

	// TODO - deal with command line arguments, save the "mode"
	


	// "size" and "num threads" into globals so threads can see them

	// initialize my matrices
	mmm_init();

	double clockstart, clockend;
	clockstart = rtclock();	// start the clock

	// << stuff I want to clock here >>

	clockend = rtclock(); // stop the clock
	printf("Time taken: %.6f sec\n", (clockend - clockstart));

	// free some stuff up

	// IDEAs: for assigning which rows to threads
	// do ceiling division, and then remove extra rows
	//	from last thread

	// Alternative: Evenly distrubute using floor division, and
	// then add one from remainder to each applicable thread
	// EX: 6 rows 4 threads. First: 0 1 2 3. Then: 0-1, 2-3, 4, 5.

	return 0;
}
