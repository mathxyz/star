#include "inner.h"
#include "Schedule.h"
#include "files.h"

void generate_netflow(const Schedule &S, const MatR &DH0, const char *path) {
    const InversibleMap &I = S.getI();
    const Sequence &p = S.getP();
    const Sequence &q = S.getQ();
    const Seqce<StarTime> &ts = S.getTs();
    const Seqce<StarTime> &te = S.getTe();

    MatR DH;
    DH.copyFrom(DH0);
    math21_operator_linear_to(1.0 / 60, DH);

    NumN n = S.size();


    MatN X(n, n);
    X = 0;

    for (NumN i = 1; i <= n; ++i) {
        for (NumN j = 1; j <= n; ++j) {
            NumR tei_h = te(i).toHour() + DH(q(i), p(j));
            NumR tsj_h = ts(j).toHour();
            if (tei_h <= tsj_h) {
                X(i, j) = 1;
            }
        }
    }
//    X.log("Netflow Matrix");

    std::ofstream myfile;
    myfile.open(path);

    NumN Vnum, Enum, s, t;
    Vnum = 2 * n + 2;
    Enum = 2 * n + (NumN) math21_operator_norm(X, 1);
    s = 2 * n + 1;
    t = 2 * n + 2;
    myfile << Vnum << " " << Enum << "\n";
    myfile << s << " " << t << " " << "3" << "\n";
    for (NumN i = 1; i <= n; ++i) {
        myfile << s << " " << 2 * i - 1 << " " << "1" << "\n";
    }
    for (NumN i = 1; i <= n; ++i) {
        myfile << 2 * i << " " << t << " " << "1" << "\n";
    }
    for (NumN i = 1; i <= n; ++i) {
        for (NumN j = 1; j <= n; ++j) {
            if (X(i, j) == 1) {
                myfile << 2 * i - 1 << " " << 2 * j << " " << "1" << "\n";
            }
        }
    }
    myfile.close();
}

// f is max flow function with vertices containing only trip nodes and values at edges equaling 1.
// f_DH is a map, mapping trip i to next DH trip.
void maxflow_to_Schedule(const Schedule &S, const MatR &DH, const Map &f_mfmc, Schedule &S_new, Map &f_DH) {
    const InversibleMap &I = S.getI();
    const Sequence &p = S.getP();
    const Sequence &q = S.getQ();
    const Seqce<StarTime> &ts = S.getTs();
    const Seqce<StarTime> &te = S.getTe();

    S.copyTo(S_new);
    NumN maxTrueTripNumber = S.getMaxTrueTripNumber();

    const SeqceN &X = f_mfmc.getX();
    const SeqceN &Y = f_mfmc.getY();
    NumN ix, iy;
    f_DH.clear();
    for (NumN k = 1; k <= X.size(); ++k) {
        ix = (X(k) + 1) / 2;
        iy = Y(k) / 2;
        if (q(ix) != p(iy)) {
            S_new.add(maxTrueTripNumber + S_new.size() +1, q(ix), te(ix), p(iy), te(ix).add_minutes(DH(q(ix), p(iy))), 1);
            f_DH.add(ix, S_new.size());
        }
    }

}

// f is max flow function with vertices containing only trip nodes and values at edges equaling 1.
// S_new is used to get true trip number.
void maxflow_to_Schedule_chains(const Schedule &S, const Schedule &S_new,
                                const Map &f_mfmc, Map &f_DH,
                                Seqce<Sequence> &chains) {
    const InversibleMap &Imap_new = S_new.getI();
    const InversibleMap &Imap = S.getI();
    const Sequence &p = S.getP();
    const Sequence &q = S.getQ();
    const Seqce<StarTime> &ts = S.getTs();
    const Seqce<StarTime> &te = S.getTe();

    chains.clear();

//    Imap.log("Imap");
//    MATH21_ASSERT(math21_operator_map_permutation(Imap))

    // fake trip number
    const Sequence &I0 = Imap.getX();
    Set I, I_tmp;
    I0.toSet(I);

    label0:
    if (I.isEmpty()) {
        return;
    }

    NumN i0, i, j;
    i0 = I(1);
    i = i0;
    I.difference(i);
    Sequence X, Y;
    X.add(i);

    const SeqceN &X2 = f_mfmc.getX();
    const SeqceN &Y2 = f_mfmc.getY();

    NumN ix, iy;

    label1:
    for (NumN k = 1; k <= X2.size(); ++k) {
        ix = (X2(k) + 1) / 2;
        iy = Y2(k) / 2;


        if (i == ix) {
            if (q(ix) != p(iy)) {
                NumN dh_num;
                dh_num = f_DH.valueAt(ix);
                X.add(dh_num);
            }
            j = iy;
            X.add(j);
            I.difference(j);
            i = j;
            goto label1;
        }
    }

    i = i0;
    label2:
    for (NumN k = 1; k <= X2.size(); ++k) {
        ix = (X2(k) + 1) / 2;
        iy = Y2(k) / 2;

        if (i == iy) {
            if (q(ix) != p(iy)) {
                NumN dh_num;
                dh_num = f_DH.valueAt(ix);
                Y.add(dh_num);
            }
            j = ix;
            Y.add(j);
            I.difference(j);
            i = j;
            goto label2;
        }
    }

    math21_operator_reverse(Y);
    Sequence c;
    math21_operator_merge(Y, X, c);

    // to real trip number
    Sequence ctrue;
    ctrue.setSize(c.size());
    for (NumN ii = 1; ii <= c.size(); ++ii) {
        ctrue.at(ii) = Imap_new.value_at_x(c(ii));
    }

    chains.push(ctrue);

    goto label0;
}

void schedule_save_chains(const Seqce<Sequence> &chains, std::ostream &io = std::cout) {
    io << "There are " << chains.size() << " chains in S.\n";
    for (NumN i = 1; i <= chains.size(); ++i) {
        io << "Chain " << i << ": ";
        chains(i).log(io);
    }
}

void schedule_maxflow_and_chains(const Schedule &S, const MatR &DH, Schedule &S_maxflow0, const char *chain_path) {
    Map f_mfmc;
    MATH21_ASSERT(chain_path)
    std::string str = chain_path;
    str = star_string_remove_suffix(str);
    MATH21_ASSERT(str.size()>0)
    xjmkdir(str.c_str());
    generate_netflow(S, DH, (str+"/gen_netflow.txt").c_str());
    mfmc_method((str+"/gen_netflow.txt").c_str(), (str+"/gen_max_flow.txt").c_str(), f_mfmc);
//    f_mfmc.log("map_mfmc");


    Map f_DH;
    maxflow_to_Schedule(S, DH, f_mfmc, S_maxflow0, f_DH);
    Seqce<Sequence> chains;
    // must be same S
    maxflow_to_Schedule_chains(S, S_maxflow0, f_mfmc, f_DH, chains);

    std::ofstream myfile;
    myfile.open(chain_path);
    schedule_save_chains(chains, myfile);
    myfile.close();
}

void schedule_chains(const Schedule &S, const MatR &DH0, const char *chain_path) {
    Schedule S_maxflow0;
    MatR DH;
    DH.copyFrom(DH0);
    math21_operator_addTo(100, DH);
    math21_operator_matrix_diagonal_set_value(DH, 0.0);
    schedule_maxflow_and_chains(S, DH, S_maxflow0, chain_path);
}