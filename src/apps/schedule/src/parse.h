#pragma once

#include "Schedule.h"


void station_name_to_num(const std::string &name, NumN &num);

void station_num_to_name(const NumN &num, std::string &name);

void parse_schedule(const char *path, Schedule &S);

void parse_schedule_data_format_2(const char *path);

void parse_schedule_data_format_2(const char *path, Schedule &S);

void parse_DH(const char *path, MatR &DH);