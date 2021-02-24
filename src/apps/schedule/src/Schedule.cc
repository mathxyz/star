#include "Schedule.h"
#include "parse.h"
#include "tools.h"

void Schedule::log(const char *s, NumB isLogType) const {
    log(std::cout, s, isLogType);
}

void Schedule::log(std::ostream &myfile, const char *s, NumB isLogType) const {
    VecN v(size());
    v.letters();
    logByOrder(myfile, s, v, isLogType);
}

void Schedule::logByStation(std::ostream &myfile, const char *s, NumB q, NumB isLogType) const {
    NumN K_size = calKSize();
    for (NumN i = 1; i <= K_size; ++i) {
        Schedule S_new;
        get_schedule_p_at_station_k(*this, i, S_new);
        S_new.log(myfile, s, isLogType);
        if(q){
            get_schedule_q_at_station_k(*this, i, S_new);
            S_new.log(myfile, s, isLogType);
        }
    }
}

void Schedule::logByOrder(std::ostream &myfile, const char *s, const VecN &vec, NumB isLogType) const {
    MATH21_ASSERT(size() == vec.size())
    if (s) {
        myfile << "Schedule " << s << ":\n";
        myfile << "[T1, T2] is [" << T1 << ", " << T2 << "]\n";
        myfile << "size: " << size() << "\n";
    }
    NumN i;
    for (NumN j = 1; j <= size(); ++j) {
        i = vec(j);
        logItem(myfile, s, i, isLogType);
    }
}

void Schedule::logItem(std::ostream &myfile, const char *s, NumN i, NumB isLogType) const {
    std::string piname, qiname;
    station_num_to_name(p(i), piname);
    station_num_to_name(q(i), qiname);
    NumN trip_number;
    trip_number = I_trip.value_at_x(i);
//    trip_number = i;
    StarTime tsi = ts.operator()(i);
    StarTime tei = te.operator()(i);
//    if(p(i)==3) {
//        tsi.addByMinute(5);
//        tei.addByMinute(5);
//    }
    myfile << trip_number << " " << piname << " " << tsi
           << " " << qiname << " " << tei;
    if (isLogType) {
        myfile << " " << tripTypes(i);
    }
    myfile << std::endl;
}

NumN Schedule::calKSize() const {
    Set A;
    A.add(p);
    A.add(q);
    return A.size();
}

const InversibleMap &Schedule::getI() const {
    return I_trip;
}

const Sequence &Schedule::getP() const {
    return p;
}

const Sequence &Schedule::getQ() const {
    return q;
}

const Seqce<StarTime> &Schedule::getTs() const {
    return ts;
}

const Seqce<StarTime> &Schedule::getTe() const {
    return te;
}

const Sequence &Schedule::getTripTypes() const {
    return tripTypes;
}

const StarTime &Schedule::getT1() const {
    return T1;
}

void Schedule::setT1(const StarTime &T1) {
    Schedule::T1 = T1;
}

const StarTime &Schedule::getT2() const {
    return T2;
}

void Schedule::setT2(const StarTime &T2) {
    Schedule::T2 = T2;
}

