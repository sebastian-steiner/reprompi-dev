#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <mpi.h>

#include "work_functions.h"


static work_func_t workfunc_list[] = {
    {"work_0", work_0 },
    {"work_lineartime_rootproc", work_lineartime_rootproc },
    {"work_rootprocwait", work_rootprocwait}
};
static const int N_WORK_FUNCS = 3;



work_function_t* get_work_function_from_name(const char* name) {
  int i;
  work_function_t *workfunc_ptr = NULL;

  if (name == NULL) {
    printf("Error: no work function name specified.\n");
    exit(1);
  }

  for (i = 0; i<N_WORK_FUNCS; i++) {
    if (strcmp(name, workfunc_list[i].name) == 0) {
      workfunc_ptr = &(workfunc_list[i].function);
      break;
    }
  }

  return workfunc_ptr;
}





void initialize_work_params(const int nelems, work_params_t* wparams) {
  int i;

  wparams->nelems = nelems;
  assert (wparams->nelems > 0);
  wparams->work_array = calloc(wparams->nelems, sizeof(double));

  for (i=0; i<wparams->nelems; i++) {
    wparams->work_array[i] = ((double)i + 1)/wparams->nelems;
  }
  wparams->root_proc = 0;
}


void cleanup_work_params(work_params_t* wparams) {
  if (wparams->nelems > 0 && wparams->work_array[0] <1) {
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (my_rank == 0) {
        printf("#\n");
    }
  }
  free(wparams->work_array);
  wparams->nelems = 0;
}



void work_0(const work_params_t* wparams) {

}


void work_lineartime_rootproc(const work_params_t* wparams) {
  int k, index1, index2;
  int my_rank, nprocs;
  long nreps;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  // root_proc has the largest number of repetitions (and thus, the longest wait)
  // the number of repetitions decreases linearly for the next ranks up to 0 repetitions for (root_proc - 1)
  nreps = wparams->nreps * (1 - (double)((my_rank - wparams->root_proc + nprocs)%nprocs)/nprocs);

  //assert(wparams->nelems > 0);
  index1 = 0;
  index2 = wparams->nelems - 1;

  for (k=0; k<nreps; k++) {
    wparams->work_array[index1] = ((long)(wparams->work_array[index1] * wparams->work_array[index2] * 1e3))% 10031;
    wparams->work_array[index2] = ((long)(wparams->work_array[index1] * wparams->work_array[index2] * 1e3))% 10031;
  }

  //printf("rank=%d nreps=%ld w1=%.10f w2=%.10f\n", my_rank, nreps, wparams->work_array[index1], wparams->work_array[index2]);

}



void work_rootprocwait(const work_params_t* wparams) {
  int k, index1, index2;
  int my_rank, nprocs;
  long nreps;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  nreps = 0;
  if (my_rank == wparams->root_proc) {
    nreps = wparams->nreps;
  }

  assert(wparams->nelems > 0);
  index1 = 0;
  index2 = wparams->nelems - 1;

  for (k=0; k<nreps; k++) {
    wparams->work_array[index1] = ((long)(wparams->work_array[index1] * wparams->work_array[index2] * 1e3))% 10031;
    wparams->work_array[index2] = ((long)(wparams->work_array[index1] * wparams->work_array[index2] * 1e3))% 10031;
  }
  //printf("rank=%d nreps=%ld w1=%.10f w2=%.10f\n", my_rank, nreps, wparams->work_array[index1], wparams->work_array[index2]);

}




