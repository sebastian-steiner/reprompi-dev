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

#ifndef REPROMPI_PARSE_ARRIVAL_PAT_OPTIONS_H_
#define REPROMPI_PARSE_ARRIVAL_PAT_OPTIONS_H_

typedef struct arrival_opt {
  char* work_type;
  long work_reps;
  int root_proc;
} reprompib_bench_arrival_options_t;

void reprompib_arrival_free_params(reprompib_bench_arrival_options_t* opts_p);

void reprompib_arrival_parse_params(reprompib_bench_arrival_options_t* opts_p, int argc, char** argv);

#endif /* REPROMPI_PARSE_ARRIVAL_PAT_OPTIONS_H_ */
