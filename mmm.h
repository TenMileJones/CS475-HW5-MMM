#ifndef MMM_H_
#define MMM_H_

// shared globals
extern unsigned int mode;
extern unsigned int size, num_threads;
extern double **A, **B, **SEQ_MATRIX, **PAR_MATRIX;

void mmm_init();
void mmm_reset(double **);
void mmm_freeup();
void mmm_seq();
void* mmm_par(void *);
double mmm_verify();
double mmm_cell(int i, int j);

typedef struct thread_args {
  int tid;    // the given thread id (0, 1, ...)
  int start; // start row of final matrix this thread is calculating
  int end;   // the row directly after end row of final matrix this thread is calculating
} thread_args;

#endif /* MMM_H_ */
