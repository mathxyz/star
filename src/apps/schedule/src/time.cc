#include "inner.h"
#include "time.h"

void StarTime::log() const {
    log(std::cout);
}

void StarTime::log(std::ostream &ostream) const {
    char str[16];
    StarTime t;
    t.set(*this);
    t.approximateToMinute();
    if (t.s != 0) {
        sprintf(str, "%ld:%.2ld:%.2ld", t.h, t.m, t.s);
    } else {
        sprintf(str, "%ld:%.2ld", t.h, t.m);
    }
    ostream << str;
}

std::ostream &operator<<(std::ostream &out, const StarTime &m) {
    m.log(out);
    return out;
}

NumR star_time_add_minutes(const StarTime &m, NumR minutes) {
    NumR h = m.toHour() + (minutes / 60.0);
    return h;
}


