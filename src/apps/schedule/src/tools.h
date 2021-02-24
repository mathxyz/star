#include "inner.h"

#include "inner.h"
#include "Schedule.h"

void get_schedule_at_station_k(const Schedule &S, NumN k, Schedule &S_new);

void get_schedule_p_at_station_k(const Schedule &S, NumN k, Schedule &S_new);

void get_schedule_q_at_station_k(const Schedule &S, NumN k, Schedule &S_new);

void save_schedule(const Schedule &S, const char *path, NumB isLogType=1);

void save_DH_matrix(const MatR &DH, const char *path);

void save_schedule_by_station_p(const Schedule &S, const char *path);
void save_schedule_by_station(const Schedule &S, const char *path);

void df_log_intervals(const Seqce<Interval> &Ms, const char *s=0, std::ostream &io = std::cout);

void df_log_d(const Step &d, const char *s = 0, std::ostream &io = std::cout, NumB timeFormat=1);


void df_log_intervals(const Seqce<Seqce<Interval> > &Mss, const char *s=0, std::ostream &io= std::cout);

void df_log_d(const Seqce<Step> &ds, const char *s = 0, std::ostream &io = std::cout, NumB timeFormat=1);

void schedule_to_Hs_Ms(const Schedule &S, NumN K_size,
                       Seqce<Seqce<Interval> > &Hss,
                       Seqce<Seqce<Interval> > &Mss,
                       Seqce<Step> &ds);

void schedule_ds_to_g(const Seqce<Step> &ds, Step &g);

NumZ schedule_get_G(const Step &g);

void df_log_G(NumZ G, const char *s = 0, std::ostream &io = std::cout);

void schedule_get_Dk(const Seqce<Step> &ds, VecZ &Dk);

NumZ schedule_get_D(const VecZ &Dk);


void df_log_Dk(const VecZ &Dk, const char *s = 0, std::ostream &io = std::cout);

void df_log_D(NumZ D, const char *s = 0, std::ostream &io = std::cout);

void schedule_dh_insertion(const Schedule &S0, const MatR &DH, Schedule& S_new, std::ostream &io = std::cout);