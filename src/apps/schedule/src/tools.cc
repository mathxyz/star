#include "inner.h"
#include "Schedule.h"
#include "files.h"

void _get_schedule_at_station_k(const Schedule &S, NumN k, Schedule &S_new, NumN which) {
    const InversibleMap &I = S.getI();
    const Sequence &p = S.getP();
    const Sequence &q = S.getQ();
    const Seqce<StarTime> &ts = S.getTs();
    const Seqce<StarTime> &te = S.getTe();
    const Sequence &tripTypes = S.getTripTypes();

    S_new.clear();

    if (which == 1) {
        for (NumN i = 1; i <= S.size(); ++i) {
            if (p(i) == k || q(i) == k) {
                S_new.add(I.value_at_x(i), p(i), ts(i), q(i), te(i), tripTypes(i));
            }
        }
    } else if (which == 2) {
        for (NumN i = 1; i <= S.size(); ++i) {
            if (p(i) == k) {
                S_new.add(I.value_at_x(i), p(i), ts(i), q(i), te(i), tripTypes(i));
            }
        }
    } else if (which == 3) {
        for (NumN i = 1; i <= S.size(); ++i) {
            if (q(i) == k) {
                S_new.add(I.value_at_x(i), p(i), ts(i), q(i), te(i), tripTypes(i));
            }
        }
    }
    S_new.setT1(S.getT1());
    S_new.setT2(S.getT2());
}

void get_schedule_at_station_k(const Schedule &S, NumN k, Schedule &S_new) {
    _get_schedule_at_station_k(S, k, S_new, 1);
}

void get_schedule_p_at_station_k(const Schedule &S, NumN k, Schedule &S_new) {
    _get_schedule_at_station_k(S, k, S_new, 2);
}

void get_schedule_q_at_station_k(const Schedule &S, NumN k, Schedule &S_new) {
    _get_schedule_at_station_k(S, k, S_new, 3);
}


void save_schedule(const Schedule &S, const char *path, NumB isLogType) {
    std::ofstream myfile;
    myfile.open(path);
    S.log(myfile, "Schedule", isLogType);
    myfile.close();
}

void save_DH_matrix(const MatR &DH, const char *path) {
    std::ofstream myfile;
    myfile.open(path);
    DH.log(myfile, "DH matrix");
    myfile.close();
}

void save_schedule_by_station(const Schedule &S, const char *path) {
    std::ofstream myfile;
    myfile.open(path);
    S.logByStation(myfile, 0, 1, 0);
    myfile.close();
}

void save_schedule_by_station_p(const Schedule &S, const char *path) {
    std::ofstream myfile;
    myfile.open(path);
    S.logByStation(myfile, 0, 0, 0);
    myfile.close();
}

void df_log_interval(const Interval &I, std::ostream &io) {
    StarTime a, b;
    a.setTimeByHour(I.left());
    b.setTimeByHour(I.right());

    if (I.isLeftClosed()) {
        io << "[";
    } else {
        io << "(";
    }
    io << a << ", " << b;
    if (I.isRightClosed()) {
        io << "]";
    } else {
        io << ")";
    }

}

void df_log_intervals(const Seqce<Interval> &Ms, const char *s, std::ostream &io) {
    StarTime a, b;
    if (s) {
        io << "intervals " << s << ":\n";
        io << "there are " << Ms.size() << " intervals.\n";
    }
    for (NumN i = 1; i <= Ms.size(); ++i) {
        const Interval &I = Ms(i);
        df_log_interval(I, io);
        if (i < Ms.size()) {
            io << " ";
        }
    }
    io << "\n";
}

void df_log_intervals(const Seqce<Seqce<Interval> > &Mss, const char *s, std::ostream &io) {
    StarTime a, b;
    if (s) {
        io << "all intervals " << s << " of S:\n";
    }
    for (NumN i = 1; i <= Mss.size(); ++i) {
        const Seqce<Interval> &Ms = Mss(i);
        const char *s2 = (std::string(s) + "_" + math21_string_to_string(i)).c_str();
        df_log_intervals(Ms, s2, io);
        if (i < Ms.size()) {
            io << "\n";
        }
    }
    io << "\n";
}

void df_log_d(const Seqce<Step> &ds, const char *s, std::ostream &io, NumB timeFormat) {
    if (s) {
        io << "deficit function " << s << " of S:\n";
        io << "there are " << ds.size() << " df.\n";
    }
    StarTime a, b;
    for (NumN i = 1; i <= ds.size(); ++i) {
        const Step &d = ds(i);
        const char *s2 = (std::string(s) + "_" + math21_string_to_string(i)).c_str();
        df_log_d(d, s2, io, timeFormat);
        if (i < ds.size()) {
            io << "\n";
        }
    }
    io << "\n";
}


void df_log_G(NumZ G, const char *s, std::ostream &io) {
    if (s) {
        io << s << " is " << G << "\n";
    }
}

// matrix format
// v = [1,2];
// A = [7.13333, 7.25;
// 7.25, 7.3;]
void df_log_d(const Step &d, const char *s, std::ostream &io, NumB timeFormat) {
    if (s) {
        io << "deficit function " << s << ":\n";
        io << "there are " << d.size() << " intervals.\n";
        io << "Is standard: " << d.isStandard() << "\n";
    }
    if (timeFormat) {
        for (NumN i = 1; i <= d.size(); ++i) {
            io << d.values.operator()(i) << ", in ";
            const Interval &I = d.getIntervals()(i);
            df_log_interval(I, io);
            io << "\n";
        }
    } else {
        io << "v = [";
        for (NumN i = 1; i <= d.size(); ++i) {
            io << d.values.operator()(i);
            if (i < d.size()) {
                io << ", ";
            }
        }
        io << "];\n";
        io << "I = [";
        for (NumN i = 1; i <= d.size(); ++i) {
            const Interval &I = d.getIntervals()(i);
            io << I.left() << ", " << I.right();
            if (i < d.size()) {
                io << "; ";
            }
        }
        io << "];\n";
    }
}


// Todo: learn std_comp, remove +1 index.
struct Compare_shedule_time {
    const Schedule &Sp;
    const Schedule &Sq;
    NumN np;

    Compare_shedule_time(const Schedule &Sp, const Schedule &Sq) : Sp(Sp), Sq(Sq) {
        np = Sp.size();
    }

    bool operator()(int i1, int i2) {
        NumN i, j;
        i = (NumN) i1 + 1;
        j = (NumN) i2 + 1;

        const Seqce<StarTime> &ts = Sp.getTs();
        const Seqce<StarTime> &te = Sq.getTe();

        if (i <= np && j <= np) {
            if (ts(i).isLessThan(ts(j))) {
                return 1;
            }
        }
        if (i <= np && j > np) {
            if (ts(i).isLessThan(te(j - np))) {
                return 1;
            }
        }
        if (i > np && j > np) {
            if (te(i - np).isLessThan(te(j - np))) {
                return 1;
            }
        }
        if (i > np && j <= np) {
            if (te(i - np).isNoBiggerThan(ts(j))) {
                return 1;
            }
        }
        return 0;
    }
};

void schedule_p_q_to_d(const Schedule &Sp, const Schedule &Sq, Step &d) {
    d.setZero();
    NumN i;
    NumR T1, T2;
    T1 = Sp.getT1().toHour();
    T2 = Sp.getT2().toHour();

    const Seqce<StarTime> &ts = Sp.getTs();
    const Seqce<StarTime> &te = Sq.getTe();
    Interval I;
    for (i = 1; i <= Sp.size(); ++i) {
        if (ts(i).toHour() < T2) {
            I.set(ts(i).toHour(), T2, 1, 1);
            d.push(1, I);
        }
    }
    for (i = 1; i <= Sq.size(); ++i) {
        if (te(i).toHour() < T2) {
            I.set(te(i).toHour(), T2, 1, 1);
            d.push(-1, I);
        }
    }
    d.toStandard();
}

void schedule_p_q_to_H_M(const Schedule &Sp, const Schedule &Sq, Seqce<Interval> &Hs, Seqce<Interval> &Ms) {
    NumN n = Sp.size() + Sq.size();
    if (n == 0) {
        return;
    }

    Hs.clear();
    Ms.clear();
    NumN np = Sp.size();
    NumN nq = Sq.size();
    VecN v;
    Compare_shedule_time comp(Sp, Sq);

    math21_operator_sort_indexes(n, v, comp);

    Interval I;
    const Seqce<StarTime> &ts = Sp.getTs();
    const Seqce<StarTime> &te = Sq.getTe();

    NumN i;

    NumR T1, T2;
    T1 = Sp.getT1().toHour();
    T2 = Sp.getT2().toHour();

    for (i = 1; i < n; ++i) {
        if (v(i) <= np && v(i + 1) > np) {
            I.set(ts(v(i)).toHour(), te(v(i + 1) - np).toHour(), 1, 1);
            Ms.push(I);
        }
    }
    if (v(i) <= np && ts(v(i)).toHour() < T2) {
        I.set(ts(v(i)).toHour(), T2, 1, 1);
        Ms.push(I);
    }

    I.set(T1, Ms(1).left(), 1, 1);
    Hs.push(I);
    for (i = 1; i < Ms.size(); ++i) {
        I.set(Ms(i).right(), Ms(i + 1).left(), 1, 1);
        Hs.push(I);
    }
    I.set(Ms(i).right(), T2, 1, 1);
    Hs.push(I);

}

void schedule_to_Hs_Ms(const Schedule &S, NumN K_size, Seqce<Seqce<Interval> > &Hss, Seqce<Seqce<Interval> > &Mss,
                       Seqce<Step> &ds) {

    const InversibleMap &I = S.getI();
    const Sequence &p = S.getP();
    const Sequence &q = S.getQ();
    const Seqce<StarTime> &ts = S.getTs();
    const Seqce<StarTime> &te = S.getTe();

    Hss.clear();
    Mss.clear();
    ds.clear();

    Hss.setSize(K_size);
    Mss.setSize(K_size);
    ds.setSize(K_size);
    for (NumN i = 1; i <= K_size; ++i) {
        Schedule Sp, Sq;
        Seqce<Interval> &Hs = Hss.at(i);
        Seqce<Interval> &Ms = Mss.at(i);

        get_schedule_p_at_station_k(S, i, Sp);
        get_schedule_q_at_station_k(S, i, Sq);
        schedule_p_q_to_H_M(Sp, Sq, Hs, Ms);
//        df_log_intervals(Ms);

        Step &d = ds.at(i);
        schedule_p_q_to_d(Sp, Sq, d);
    }
}

void schedule_ds_to_g(const Seqce<Step> &ds, Step &g) {
    g.setZero();
    Step g_tmp;
    for (NumN i = 1; i <= ds.size(); ++i) {
        const Step &d = ds(i);
        g.add(d, g_tmp);
        g.swap(g_tmp);
    }
}

// get max
NumR math21_function_max_step(const Step &f) {
    const VecR &v = f.getValues();
    return math21_operator_max(v);
}

NumZ schedule_get_G(const Step &g) {
    return (NumZ) math21_function_max_step(g);
}

void schedule_get_Dk(const Seqce<Step> &ds, VecZ &Dk) {
    Dk.setSize(ds.size());
    for (NumN i = 1; i <= ds.size(); ++i) {
        const Step &d = ds(i);
        Dk(i) = (NumZ) math21_function_max_step(d);
        Dk(i) = xjmax(Dk(i), (NumZ) 0);
    }
}

NumZ schedule_get_D(const VecZ &Dk) {
    return (NumZ) math21_operator_sum(Dk, 1);
}


void df_log_Dk(const VecZ &Dk, const char *s, std::ostream &io) {
    Dk.log(io, "Dk");
}

void df_log_D(NumZ D, const char *s, std::ostream &io) {
    io << "D is " << D << "\n";
}


void schedule_get_first_M(const Step &d, NumZ Dk,
                          const Seqce<Interval> &Ms, NumR t0,
                          Interval &Mf) {
    NumN n = Ms.size();
    for (NumN i = 1; i <= n; ++i) {
        const Interval &I = Ms(i);
        if (I.left() < t0) {
            continue;
        }
        if (d.valueAt(I.left()) == Dk) {
            Mf.set(I);
            return;
        }
    }
    MATH21_ASSERT(0, "no first M after t0")
}

void schedule_get_first_Ms(const Seqce<Step> &ds, const VecZ &Dk,
                           const Seqce<Seqce<Interval> > &Mss, NumR t0,
                           Seqce<Interval> &Mfs) {
    NumN n = ds.size();
    Mfs.setSize(n);
    for (NumN i = 1; i <= n; ++i) {
        schedule_get_first_M(ds(i), Dk(i), Mss(i), t0, Mfs.at(i));
    }
}

void schedule_get_last_M(const Step &d, NumZ Dk,
                         const Seqce<Interval> &Ms,
                         Interval &Ml) {
    NumN n = Ms.size();
    for (NumN i = n; i >= 1; --i) {
        const Interval &I = Ms(i);
        if (d.valueAt(I.left()) == Dk) {
            Ml.set(I);
            return;
        }
    }
    MATH21_ASSERT(0, "no last M")
}

void schedule_get_last_Ms(const Seqce<Step> &ds, const VecZ &Dk,
                          const Seqce<Seqce<Interval> > &Mss,
                          Seqce<Interval> &Mls) {
    NumN n = ds.size();
    Mls.setSize(n);
    for (NumN i = 1; i <= n; ++i) {
        schedule_get_last_M(ds(i), Dk(i), Mss(i), Mls.at(i));
    }
}

// URDHC version 1
void schedule_URDHC_v1(const Schedule &S,
                       const MatR &DH,
                       const Set &K,
                       NumN u,
                       NumN q,
                       NumR sq,
                       Schedule &S_DH,
                       NumN maxTrueTripNumber,
                       const Seqce<Step> &ds,
                       const VecZ &Dk,
                       const Seqce<Seqce<Interval> > &Hss,
                       const Seqce<Seqce<Interval> > &Mss,
                       NumN method,
                       const Seqce<Interval> &Mfs,
                       const Seqce<Interval> &Mls,
                       std::ostream &io) {
    Set K2;
    Set K1;

    label1:
    K1.clear();
    K1.add(q);
//    m21log("q", q);
//    m21log("sq", sq);
    K.difference(K1, K2);
    Sequence x1;
    Sequence x2;
    SequenceR evs;
//    K2.log("K2");
    for (NumN i = 1; i <= K2.size(); ++i) {
        NumN v = K2(i);
        for (NumN j = 1; j <= Hss(v).size(); ++j) {
            NumR ev = Hss(v)(j).left();

            const Sequence &p_dh = S_DH.getP();
            const Sequence &q_dh = S_DH.getQ();
            const Seqce<StarTime> &ts = S_DH.getTs();
            NumB flag = 0;
            for (NumN i2 = 1; i2 <= S_DH.size(); ++i2) {
                if (method == 11) {
                    if (p_dh(i2) == v) {
                        NumR t = ts(i2).toHour();
                        if (math21_operator_num_isEqual(ev, t, 0.00001)) {
                            flag = 1;
                            break;
                        }
                    }
                } else if (method == 12) {
//                    if (p_dh(i2) == v || q_dh(i2) == v) {
////                        NumR t = ts(i2).toHour();
////                        if (math21_operator_num_isEqual(ev, t, 0.00001)) {
//                        flag = 1;
//                        break;
////                        }
//                    }

                    if (q_dh(i2) == v) {
                        if (ev >= Mls(v).right()) {

                        } else {
                            flag = 1;
                            break;
                        }
                    } else if (p_dh(i2) == v) {
                        NumR t = ts(i2).toHour();
                        if (!math21_operator_num_isLarger(ev, t, 0.00001)) {
                            flag = 1;
                            break;
                        }
                    }
                }
            }
            if (flag) {
                break;
            }

            if (ev + DH(v, q) <= sq) {
                x1.add(v);
                x2.add(j);
                evs.add(ev);
            } else {
                break;
            }
        }
    }
    if (x1.isEmpty()) {
        S_DH.clear();
        return;
    }
    NumN R;
    R = 2;

    VecN idx;
    Compare_SequenceR comp(evs);
    math21_operator_sort_indexes(x1.size(), idx, comp);

    NumN x_id = idx(idx.size());

    NumN p = x1(x_id);
    NumN j = x2(x_id);
    NumR ep = evs(x_id);
    StarTime ts;
    ts.setTimeByHour(ep);
    StarTime te;
    te.setTimeByHour(ep + DH(p, q));
    S_DH.add(maxTrueTripNumber + S_DH.size() + 1, p, ts, q, te, 1);

//    S_DH.logItem(std::cout, "", S_DH.size());

    if (method == 11) {
        if (j == Hss(p).size()) {
            return;
        }

        sq = Hss(p)(j).right();
        q = p;
    } else if (method == 12) {
        if (ep >= Mls(p).right()) {
            return;
        }

        Interval Mf;
        schedule_get_first_M(ds(p), Dk(p), Mss(p), ep, Mf);

        sq = Mf.left();
        q = p;
    }

    goto label1;
}

// URDHC version 2
NumB schedule_URDHC_v2(const Schedule &S,
                       const MatR &DH,
                       const Set &K,
                       NumN u,
                       NumN q,
                       NumR sq,
                       Schedule &S_DH,
                       NumN maxTrueTripNumber,
                       const Seqce<Step> &ds,
                       const VecZ &Dk,
                       const Seqce<Seqce<Interval> > &Hss,
                       const Seqce<Seqce<Interval> > &Mss,
                       NumN method,
                       const Seqce<Interval> &Mfs,
                       const Seqce<Interval> &Mls,
                       std::ostream &io) {

    Set K2;
    m21log("q", q);
    m21log("sq", sq);
    K.difference(q, K2);

    Sequence x1;
    Sequence x2;
    SequenceR evs;
//    K2.log("K2");
    for (NumN i = 1; i <= K2.size(); ++i) {
        NumN v = K2(i);
        for (NumN j = 1; j <= Hss(v).size(); ++j) {
            NumR ev = Hss(v)(j).left();

            const Sequence &p_dh = S_DH.getP();
            const Sequence &q_dh = S_DH.getQ();
            const Seqce<StarTime> &ts = S_DH.getTs();
            const Seqce<StarTime> &te = S_DH.getTe();
            NumB flag = 0;
            for (NumN i2 = 1; i2 <= S_DH.size(); ++i2) {
                if (method == 11) {
                    if (p_dh(i2) == v) {
                        NumR t = ts(i2).toHour();
                        if (math21_operator_num_isEqual(ev, t, 0.00001)) {
                            flag = 1;
                            break;
                        }
                    }
                } else if (method == 12) {
                    if (q_dh(i2) == v) {
                        if (ev >= Mls(v).right()) {

                        } else {
                            flag = 1;
                            break;
                        }
                    } else if (p_dh(i2) == v) {
//                        NumR t = ts(i2).toHour();
//                        if (!math21_operator_num_isLarger(ev, t, 0.00001)) {
                        flag = 1;
                        break;
//                        }
                    }
                }
            }
            if (flag) {
                break;
            }

            if (ev + DH(v, q) <= sq) {
                x1.add(v);
                x2.add(j);
                evs.add(ev);
            } else {
                break;
            }
        }
    }
    if (x1.isEmpty()) {
        if (!S_DH.isEmpty()) {
            S_DH.removeLast();
        }
        return 0;
    }

    NumN R;
    R = 2;

    // no rule, we check everyone.
    for (NumN x_id = 1; x_id <= evs.size(); ++x_id) {

        NumN p = x1(x_id);
        NumN j = x2(x_id);
        NumR ep = evs(x_id);
        StarTime ts;
        ts.setTimeByHour(ep);
        StarTime te;
        te.setTimeByHour(ep + DH(p, q));
        S_DH.add(maxTrueTripNumber + S_DH.size() + 1, p, ts, q, te, 1);

//    S_DH.logItem(std::cout, "", S_DH.size());

        NumR sp;
        if (method == 11) {
            if (j == Hss(p).size()) {
                return 1;
            }

            sp = Hss(p)(j).right();
        } else if (method == 12) {
            if (ep >= Mls(p).right()) {
                if (p != u) {
                    return 1;
                } else {
                    continue;
                }
            }

            Interval Mf;
            schedule_get_first_M(ds(p), Dk(p), Mss(p), ep, Mf);

            sp = Mf.left();
        } else {
            MATH21_ASSERT(0)
        }

        NumB result;
        result = schedule_URDHC_v2(S,
                                   DH,
                                   K,
                                   u,
                                   p,
                                   sp,
                                   S_DH,
                                   maxTrueTripNumber,
                                   ds,
                                   Dk,
                                   Hss,
                                   Mss,
                                   method,
                                   Mfs,
                                   Mls,
                                   io);

        if (result == 1) {
            return 1;
        }
    }
    if (!S_DH.isEmpty()) {
        S_DH.removeLast();
    }
    return 0;
}

void schedule_URDHC(const Schedule &S,
                    const MatR &DH0,
                    const Set &K,
                    NumN u,
                    NumR su,
                    Schedule &S_DH,
                    NumN maxTrueTripNumber,
                    const Seqce<Step> &ds,
                    const VecZ &Dk,
                    const Seqce<Seqce<Interval> > &Hss,
                    const Seqce<Seqce<Interval> > &Mss,
                    NumN method,
                    const Seqce<Interval> &Mfs,
                    const Seqce<Interval> &Mls,
                    std::ostream &io) {
    m21log("schedule_URDHC");
    m21log("u", u);
    NumN q;
    q = u;
    S_DH.clear();

    NumR sq;
    sq = su;

    MatR DH;
    DH.copyFrom(DH0);
    math21_operator_linear_to(1.0 / 60, DH);

    schedule_URDHC_v1(S,
                      DH,
                      K,
                      u,
                      q,
                      sq,
                      S_DH,
                      maxTrueTripNumber,
                      ds,
                      Dk,
                      Hss,
                      Mss,
                      method,
                      Mfs,
                      Mls,
                      io);
}

void schedule_dh_insertion(const Schedule &S0, const MatR &DH,
                           Schedule &S_new, std::ostream &io) {
    //
    io << "start " << __FUNCTION__ << std::endl;
    Schedule &S = S_new;
    S0.copyTo(S);

    Set W;
    Set K;
    VecN K_v(DH.nrows());
    K_v.letters();
    math21_operator_vec_to_set(K_v, K);

    while (1) {

        label1:
        //
        Seqce<Seqce<Interval> > Hss;
        Seqce<Seqce<Interval> > Mss;
        Seqce<Step> ds;
        VecZ Dk;
        NumZ D;
        Step g;
        NumZ G;
        io << "<<<<<<<<<<<<<<<<<<\n";
        //
        schedule_to_Hs_Ms(S, K.size(), Hss, Mss, ds);
//        df_log_intervals(Mss, "Mss", io);
//        df_log_intervals(Hss, "Hss", io);
        df_log_d(ds, "ds", io, 0);

        schedule_get_Dk(ds, Dk);
        D = schedule_get_D(Dk);
        df_log_Dk(Dk, "Dk", io);

        schedule_ds_to_g(ds, g);
        df_log_d(g, "g", io, 0);
        G = schedule_get_G(g);
        io << "<<<<<<\n";
        df_log_D(D, "D", io);
        df_log_G(G, "G", io);
        io.flush();

        //
        label2:
        if (G == D || W.size() == K.size()) {
            break;
        }

        Set K2;
        K.difference(W, K2);

        // get u, su
        NumN u;
        NumN u_last = 0; // for beauty, we choose different u.
        NumR su;
        Seqce<Interval> Mfs;
        Seqce<Interval> Mls;
//        NumN method = 11;
        NumN method = 12;
        if (method == 11) {
            Sequence x1;
            SequenceR x2;
            for (NumN i = 1; i <= K2.size(); ++i) {
                NumN k = K2(i);
                x1.add(k);
                NumR length = Hss(k)(1).length();
                x2.add(length);
            }
            VecN idx;
            Compare_SequenceR comp(x2);
            math21_operator_sort_indexes(x1.size(), idx, comp);
            NumN x_id = idx(idx.size());
            u = x1(x_id);
            su = Mss(u)(1).left();

//        x1.log("x1");
//        x2.log("x2");
//        idx.log("idx");
        } else if (method == 12) {

            schedule_get_first_Ms(ds, Dk, Mss,
                                  S.getT1().toHour(),
                                  Mfs);
            schedule_get_last_Ms(ds, Dk, Mss,
                                 Mls);

            if (1) {
                Sequence x1;
                SequenceR x2;
                for (NumN i = 1; i <= K2.size(); ++i) {
                    NumN k = K2(i);
                    x1.add(k);
                    NumR length = Mfs(k).left();
                    x2.add(length);
                }
                VecN idx;
                Compare_SequenceR comp(x2);
                math21_operator_sort_indexes(x1.size(), idx, comp);
                NumN x_id;
                if (u == u_last && idx.size() >= 2) {
                    x_id = idx(idx.size() - 1);
                } else {
                    x_id = idx(idx.size());
                }
                u = x1(x_id);
                su = Mfs(u).left();
            } else {
                u = K2(1);
                su = Mfs(u).left();
            }

        }

        //
        W.add(u);

        Schedule S_DH;
        NumN maxTrueTripNumber = S.getMaxTrueTripNumber();
        schedule_URDHC(S, DH, K, u, su,
                       S_DH,
                       maxTrueTripNumber,
                       ds, Dk,
                       Hss, Mss,
                       method, Mfs, Mls, io);

        if (S_DH.size() == 0) {
            goto label2;
        }

        // check cost, pass
        S.add(S_DH);
        W.clear();
        u_last = u;
        goto label1;
    }
}