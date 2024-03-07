#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "mmm.h"

/**
 * Allocate and initialize the matrices on the heap. Populate
 * the input matrices with random numbers from 0 to 99
 */
void mmm_init() {
	// Allocate matrices
	A = (double **) malloc(size * sizeof(double*));
	B = (double **) malloc(size * sizeof(double*));
	SEQ_MATRIX = (double **) malloc(size * sizeof(double*));
	PAR_MATRIX = (double **) malloc(size * sizeof(double*));
	
	srand((unsigned)time(NULL));	// seed the random number generator
 	// initialize A and B with random values between 0 and 99
	// initialize SEQ_MATRIX and PAR_MATRIX with 0s
	int i = 0, j = 0;
	while (i < size){
		A[i] = (double *) malloc(size * sizeof(double));
		B[i] = (double *) malloc(size * sizeof(double));
		SEQ_MATRIX[i] = (double *) malloc(size * sizeof(double));
		PAR_MATRIX[i] = (double *) malloc(size * sizeof(double));
		while (j < size){
			A[i][j] = rand() % 100;
			B[i][j] = rand() % 100;
			SEQ_MATRIX[i][j] = 0;
			PAR_MATRIX[i][j] = 0;
			j++;
		}
		i++;
	}
}

/**
 * Reset a given matrix to zeroes (their size is in the global var)
 * @param matrix pointer to a 2D array
 */
void mmm_reset(double **matrix) {
	int i = 0, j = 0;
	while (i < size){
		while (j < size){
			matrix[i][j] = 0;
			j++;
		}
		i++;
	}
}

/**
 * Free up memory allocated to all matrices
 * (their size is in the global var)
 */
void mmm_freeup() {
	int i = 0;
    while (i < size) {
		free(A[i]);
		free(B[i]);
		free(SEQ_MATRIX[i]);
		free(PAR_MATRIX[i]);
        i++;
	}
    free(A);
	free(B);
	free(SEQ_MATRIX);
	free(PAR_MATRIX);
}

/**
 * Sequential MMM (size is in the global var)
 */
void mmm_seq() {
	// TODO - code to perform sequential MMM
}

/**
 * Parallel MMM
 */
void *mmm_par(void *args) {
	// TODO - code to perform parallel MMM
}

/**
 * Verifies the correctness between the matrices generated by
 * the sequential run and the parallel run.
 *
 * @return the largest error between two corresponding elements
 * in the result matrices
 */
double mmm_verify() {
	// TODO
	// You may find the math.h function fabs() to be useful here (absolute value)
	return -1;
}
