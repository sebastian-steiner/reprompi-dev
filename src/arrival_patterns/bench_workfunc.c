/*  ReproMPI Benchmark
 *
 *  Copyright 2015 Alexandra Carpen-Amarie, Sascha Hunold
 Research Group for Parallel Computing
 Faculty of Informatics
 Vienna University of Technology, Austria

 <license>
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 </license>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_sort.h>

#include "reprompi_bench/misc.h"
#include "reprompi_bench/sync/time_measurement.h"
#include "work_functions.h"


int main(int argc, char* argv[]) {
  long i, j;
  double* trun_sec;
  work_function_t* work_func;
  work_params_t wparams;
  long min_work_iter, step_work_iter;
  double current_work_latency;
  long nreps = 1000;
  double max_work_latency_us = 0;
  double max_work_iter;

  MPI_Init(&argc, &argv);

  if (argc < 3) {
    printf("\nUSAGE: %s nreps max_work_latency_us \n", argv[0]);
    return 0;
  }

  nreps = atol(argv[1]);
  max_work_latency_us = atof(argv[2]);

  if (nreps < 1) {
    printf("\nnreps should be >0 \n");
    return 0;
  }

  // set work function
  initialize_work_params(2, &wparams);
  work_func = get_work_function_from_name("work_lineartime_rootproc");
  if (work_func == NULL) {
    reprompib_print_error_and_exit("Invalid work function");
  }


  min_work_iter = 1;
  max_work_iter = 1e10;
  step_work_iter = 2;

  printf("%10s %16s %16s %16s %16s\n", "work_nreps", "min_time_us", "max_time_us", "mean_time_us", "median_time_us");

  i = min_work_iter;
  current_work_latency = 0;
  while (current_work_latency < max_work_latency_us && i < max_work_iter) {
    trun_sec = (double*) calloc(nreps, sizeof(double));

    wparams.nreps = i;

    for (j = 0; j < nreps; j++) {
      trun_sec[j] -= get_time();
      (*work_func)(&wparams);
      trun_sec[j] += get_time();
    }

    current_work_latency = gsl_stats_mean(trun_sec, 1, nreps) * 1e6;
    gsl_sort(trun_sec, 1, nreps);
    printf("%10ld %16.8f %16.8f %16.8f %16.8f\n", i, trun_sec[0] * 1e6, trun_sec[nreps-1] * 1e6,
        current_work_latency,
        gsl_stats_median_from_sorted_data (trun_sec, 1, nreps) * 1e6);
    i = i * step_work_iter;

    free(trun_sec);
  }

  MPI_Finalize();
  return 0;
}
