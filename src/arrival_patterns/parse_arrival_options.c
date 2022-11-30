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
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <mpi.h>

#include "reprompi_bench/misc.h"
#include "parse_arrival_options.h"

static const int OUTPUT_ROOT_PROC = 0;

enum arrival_pat_getopt_ids {
  ROOTPROC,
  WORKTYPE,
  WORKREPS
};

static const struct option arrival_pat_long_options[] = {
    { "work-root", required_argument, 0, ROOTPROC },
    { "work-type", required_argument, 0, WORKTYPE },
    { "work-reps", required_argument, 0, WORKREPS },
    { 0, 0, 0, 0 }
};
static const char arrival_pat_opts_str[] = "h";


static void init_parameters(reprompib_bench_arrival_options_t* opts_p) {
  opts_p->work_type = NULL;
  opts_p->work_reps = 0;
}

void reprompib_arrival_pat_print_help(int argc, char** argv) {
  int my_rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  if (my_rank == OUTPUT_ROOT_PROC) {
    printf("\nUSAGE: %s [options]\n", argv[0]);
    printf("options:\n");
  }

  if (my_rank == OUTPUT_ROOT_PROC) {
    printf("%-40s %-40s\n", "--work-root=<root_proc>", "root process for the work function");
    printf("%-40s %-40s\n", "--work-type=<work_func_name>", "work function to use (work_0, work_lineartime_rootproc)");
    printf("%-40s %-40s\n", "--work-reps=<nrep>", "number of repetitions in the work loop");

    printf(
        "\nEXAMPLES: mpirun -np 2 %s --calls-list=MPI_Bcast --msizes-list=8,512,1024 --work-type=work_lineartime_rootproc --work-reps=1000 --nrep=2 \n", argv[0]);
    printf("\n\n");
  }
}



void reprompib_arrival_parse_params(reprompib_bench_arrival_options_t* opts_p, int argc, char** argv) {
  int c, err;
  int printhelp = 0;
  int my_rank, nprocs;
  long rootl;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  init_parameters(opts_p);

  opterr = 0; // ignore invalid options
  while (1) {

    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long(argc, argv, arrival_pat_opts_str, arrival_pat_long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {
    case WORKTYPE:
      opts_p->work_type = strdup(optarg);
      break;
    case WORKREPS:
      err = reprompib_str_to_long(optarg, &(opts_p->work_reps));
      if (err || opts_p->work_reps <= 0) {
        reprompib_print_error_and_exit("Invalid nreps in the work loop (should be positive)");
      }
      break;
    case ROOTPROC:
      err = reprompib_str_to_long(optarg, &(rootl));

      MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
      if (err || rootl < 0 || rootl >= nprocs) {
        reprompib_print_error_and_exit("Invalid root_proc for the work function");
      }
      opts_p->root_proc = rootl;
      break;
    case 'h':
      printhelp = 1;
      break;

    case '?':
      break;
    }
  }

  if (printhelp) {
    reprompib_arrival_pat_print_help(argc, argv);
  }

  if (opts_p->work_reps <= 0) {
    reprompib_print_error_and_exit("Invalid number of repetitions (should be >0)");
  }

  if (opts_p->work_type == NULL) {
    reprompib_print_error_and_exit("The work function was not specified");
  }

  optind = 1; // reset optind to enable option re-parsing
  opterr = 1; // reset opterr to catch invalid options
}


void reprompib_arrival_free_params(reprompib_bench_arrival_options_t* opts_p) {
  free(opts_p->work_type);
}









