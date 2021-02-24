#include "inner.h"
#include "files.h"

void schedule_process(const char *S_path, const char *DH_path) {
    Schedule S0;
    MatR DH;

    parse_schedule(S_path, S0);

//    S0.log("S0");

    parse_DH(DH_path, DH);

    Schedule S_maxflow0, S_maxflow2;
    save_schedule(S0, "gen_S.txt", 0);
    save_DH_matrix(DH, "gen_DH.txt");
//    save_schedule_by_station_p(S0, "gen_S_Kp.txt");
//    save_schedule_by_station(S0, "gen_S_K.txt");
//    schedule_chains(S0, DH, "gen_Chains.txt");
//    schedule_maxflow_and_chains(S0, DH, S_maxflow0, "gen_Chains_maxflow.txt");
//    S_maxflow.log("gen_S_maxflow");
//    save_schedule(S_maxflow0, "gen_S_maxflow.txt");

    //
    const char *output_path = "gen_info_DH.txt";
    std::ofstream myfile;
    myfile.open(output_path);

//    Schedule &S = S_maxflow0;
    Schedule &S = S0;
    Schedule S_DH;
    schedule_dh_insertion(S, DH, S_DH, myfile);
    myfile.close();
    save_schedule(S_DH, "gen_S_DH.txt");
//    schedule_chains(S_DH, DH, "gen_Chains_DH.txt");
//    S_after_DH.log("S_after_DH");


    //
//    output_path = "gen_info_maxflow_DH.txt";
//    myfile.open(output_path);

//    Schedule &S2 = S_maxflow0;
//    Schedule S_DH2;
//    schedule_dh_insertion(S2, DH, S_DH2, myfile);
//    save_schedule(S_DH2, "gen_S_maxflow_DH.txt");
//    schedule_chains(S_DH2, DH, "gen_Chains_maxflow_DH.txt");
//    myfile.close();
}
