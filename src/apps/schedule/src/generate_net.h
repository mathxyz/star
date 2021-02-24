#include "inner.h"

void generate_netflow(const Schedule &S, const MatR &DH, const char *path);

void maxflow_to_Schedule(const Schedule &S, const MatR &DH, const Map &f_mfmc, Schedule &S_new, Map &f_DH);

void schedule_maxflow_and_chains(const Schedule &S, const MatR &DH, Schedule &S_maxflow0, const char *chain_path);

void schedule_chains(const Schedule &S, const MatR &DH0, const char *chain_path);