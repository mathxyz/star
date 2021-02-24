#include "inner.h"
#include "listring.h"
#include "files.h"

namespace star {
// whether t[i] appears in s. return -1 , 0 , 1;
    int liregex2_string(char *s, const Seqce<std::string> &patterns, int fr, int to) {
        int re = 0;
        NumN i = 1;
        NumN tn = patterns.size();
        while ((i <= tn) && (re = liregex3(s, (patterns(i++).c_str()), fr, to)) == 0);
        return re;
    }
}