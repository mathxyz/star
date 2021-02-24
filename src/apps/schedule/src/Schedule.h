#pragma once

#include "inner.h"
#include "time.h"


struct Schedule {
private:
    void init() {
        setT1(StarTime(2, 0, 0));
        setT2(StarTime(25, 0, 0));
    }

public:
    InversibleMap I_trip; // index and trip number
    Sequence p;
    Sequence q;
    Seqce<StarTime> ts;
    Seqce<StarTime> te;
    Sequence tripTypes; // 0: normal, 1: deadheading.
    StarTime T1;
    StarTime T2;

    const StarTime &getT1() const;

    void setT1(const StarTime &T1);

    const StarTime &getT2() const;

    void setT2(const StarTime &T2);


    void clear() {
        I_trip.clear();
        p.clear();
        q.clear();
        ts.clear();
        te.clear();
        tripTypes.clear();
    }

    void removeLast() {
        I_trip.remove(size());
        p.removeLast();
        q.removeLast();
        ts.removeLast();
        te.removeLast();
        tripTypes.removeLast();
    }

    Schedule() {
        init();
    }

    virtual ~Schedule() {
        clear();
    }

    NumN size() const {
        return I_trip.size();
    }

    NumN isEmpty() const {
        if(I_trip.size()==0){
            return 1;
        } else{
            return 0;
        }
    }

    void add(const Schedule &S) {
        const InversibleMap &I = S.getI();
        const Sequence &p = S.getP();
        const Sequence &q = S.getQ();
        const Seqce<StarTime> &ts = S.getTs();
        const Seqce<StarTime> &te = S.getTe();
        const Sequence &tripTypes = S.getTripTypes();

        for (NumN i = 1; i <= S.size(); ++i) {
            add(I.valueAt(i), p(i), ts(i), q(i), te(i), tripTypes(i));
        }
    }

    void add(NumN i, NumN pi, const StarTime &tsi, NumN qi, const StarTime &tei, NumN trip_type = 0) {
        I_trip.add(I_trip.size() + 1, i);
        p.add(pi);
        ts.push(tsi);
        q.add(qi);
        te.push(tei);
        tripTypes.add(trip_type);
    }

    NumN getMaxTrueTripNumber()const{
        Set B;
        I_trip.getY().toSet(B);
        return B.max();
    }
    // Todo: Make sure I contain 1. We add trip to S, not S at some station.
    // deprecate
    void add(NumN pi, const StarTime &tsi, NumN qi, const StarTime &tei, NumN trip_type = 0) {
//        MATH21_ASSERT_SLOW(math21_operator_map_permutation(I_trip))
        MATH21_ASSERT(0)
        NumN i = I_trip.getYSet().max() + 1;
        add(i, pi, tsi, qi, tei, trip_type);
    }

    void copyTo(Schedule &S) const {
        S.clear();
        I_trip.copyTo(S.I_trip);
        p.copyTo(S.p);
        q.copyTo(S.q);
        ts.copyTo(S.ts);
        te.copyTo(S.te);
        tripTypes.copyTo(S.tripTypes);
        S.setT1(T1);
        S.setT2(T2);
    }

    const Sequence &getTripTypes() const;

    NumN calKSize()const;

    void log(const char *s = 0, NumB isLogType=1) const;

    void log(std::ostream &ostream, const char *s, NumB isLogType=1) const;

    void logByStation(std::ostream &ostream, const char *s, NumB q, NumB isLogType=1) const;

    void logByOrder(std::ostream &ostream, const char *s, const VecN &vec, NumB isLogType=1) const;

    void logItem(std::ostream &ostream, const char *s, NumN i, NumB isLogType=1) const;

    const Seqce<StarTime> &getTs() const;

    const Sequence &getP() const;

    const Sequence &getQ() const;

    const InversibleMap &getI() const;

    const Seqce<StarTime> &getTe() const;


};

