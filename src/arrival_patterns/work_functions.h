
#ifndef REPROMPI_WORK_FUNCTIONS_H_
#define REPROMPI_WORK_FUNCTIONS_H_


typedef struct work_params_struct {
    double* work_array;
    long nelems;
    long nreps;
    int root_proc;

} work_params_t;


typedef void (*work_function_t)(const work_params_t* wparams);

typedef struct work_func_struct {
    char* name;
    work_function_t function;
} work_func_t;

work_function_t* get_work_function_from_name(const char* name);

void initialize_work_params(const int nelems, work_params_t* wparams);
void cleanup_work_params(work_params_t* wparams);

void work_0(const work_params_t* wparams);
void work_lineartime_rootproc(const work_params_t* wparams);
void work_rootprocwait(const work_params_t* wparams);

#endif /* REPROMPI_WORK_FUNCTIONS_H_ */


