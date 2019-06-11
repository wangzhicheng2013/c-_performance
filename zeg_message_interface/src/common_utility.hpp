/*****************************************************************************
*  common utility                                                            *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     common_utility.hpp                                              *
*  @brief    common utility functions                                        *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-06-10                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/06/03 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*  2019/06/10 | 1.0.0     |                | Update system::std              *
*----------------------------------------------------------------------------*
*****************************************************************************/
#ifndef SRC_COMMON_UTILITY_HPP_
#define SRC_COMMON_UTILITY_HPP_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
template <typename T>
void merge_vector(const vector<T>&v0, vector<T>&v1) {
	for_each(begin(v0), end(v0), [&v1](auto &e) {
		v1.emplace_back(e);
	});
}
inline void run_program(const char* path) {
	cout << std::system(path) << endl;
    usleep(5000);
}
void kill_program(const char* program_name) {
	char cmd[128] = "";
	snprintf(cmd, sizeof(cmd), "ps x | grep ./%s | grep -v grep | awk \'{print $1}\'", program_name);
	FILE* fp = popen(cmd, "r");
	if (!fp) {
			cerr << cmd << " failed...!" << endl;
			return;
	}
	char buf[64] = "";
	while (fgets(buf, sizeof(buf), fp)) {
	}
	pclose(fp);
	snprintf(cmd, sizeof(cmd), "kill -9  %s", buf);
	cout << cmd << endl;
	cout << std::system(cmd) << endl;
	usleep(5000);
}

#endif /* SRC_COMMON_UTILITY_HPP_ */
