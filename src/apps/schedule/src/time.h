#pragma once

#include "inner.h"

using namespace star;

struct StarTime {
public:
    NumN h;
    NumN m;
    NumN s;

    void init() {
        h = 0;
        m = 0;
        s = 0;
    }

    void set(NumN h, NumN m, NumN s) {
        this->h = h;
        this->m = m;
        this->s = s;
    }

    void set(const std::string &str, const std::string &format) {
        NumN h, m, s;
        h = 0;
        m = 0;
        s = 0;
        if (format == "%d:%d") {
            sscanf(str.c_str(), format.c_str(), &h, &m);
        }
        set(h, m, s);
    }

    StarTime(NumN h, NumN m, NumN s) {
        init();
        set(h, m, s);
    }

    StarTime() {
        init();
    }

    virtual ~StarTime() {
    }


    void log() const;

    void log(std::ostream &ostream) const;

    NumR toHour() const {
        NumR x = h + (m / 60.0) + (s / (3600.0));
        return x;
    }

    NumR toSeconds() const {
        NumR x = h * 3600 + m * 60 + s;
        return x;
    }

    void approximateToMinute() {
        if (s > 30) {
            addBySecond(30);
            s = 0;
        } else {
            s = 0;
        }
    }

    void addByMinute(NumR m) {
        addBySecond(m * 60);
    }

    void addBySecond(NumR s) {
        s += toSeconds();
        setTimeBySecond(s);
    }

    StarTime add_minutes(NumR minutes) const {
        NumN s = (NumN) (minutes * 60);
        s += toSeconds();
        StarTime time;
        time.setTimeBySecond(s);
        return time;
    }

    void setTimeByHour(NumR h) {
        setTimeBySecond(h * 3600);
    }

    void setTimeByMinute(NumR m) {
        setTimeBySecond(m * 60);
    }

    void setTimeBySecond(NumR s) {
        NumN h, m;
        h = (NumN) (s / 3600);
        s -= h * 3600;
        m = (NumN) (s / 60);
        s -= m * 60;
        StarTime time(h, m, (NumN) s);
        set(time);
    }

    void set(const StarTime &time) {
        set(time.h, time.m, time.s);
    }

    void add(const StarTime &time) {

    }

    NumB isLessThan(const StarTime &time) const {
        if (h < time.h) {
            return 1;
        } else if (h == time.h && m < time.m) {
            return 1;
        } else if (h == time.h && m == time.m && s < time.s) {
            return 1;
        }
        return 0;
    }

    NumB isNoBiggerThan(const StarTime &time) const {
        if (isLessThan(time) || isEqual(time)) {
            return 1;
        }
        return 0;
    }

    NumB isEqual(const StarTime &time) const {
        if (h == time.h && m == time.m && s == time.s) {
            return 1;
        }
        return 0;
    }
};

std::ostream &operator<<(std::ostream &out, const StarTime &m);

