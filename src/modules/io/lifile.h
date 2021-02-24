#pragma once

namespace star {
    int lisearchSatisfy_string(char *s, const Seqce <std::string> &patterns, int fr, int to);

    int _lils2(void *container, const char *par, const Seqce <std::string> &patterns, int recTimes,
               int tf, int td,
               int (*isSatisfied)(char *s, const Seqce <std::string> &patterns, int fr,
                                  int to),
               int (*tackle)(void *container, const char *par, char *entry,
                             int isDir));

// for main function.
    int lils(int ac, const char **av);
}