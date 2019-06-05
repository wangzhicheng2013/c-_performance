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
*  @date     2019-06-05                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/06/03 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*****************************************************************************/
#ifndef SRC_COMMON_UTILITY_HPP_
#define SRC_COMMON_UTILITY_HPP_
#include <vector>
#include <algorithm>
using namespace std;
template <typename T>
void merge_vector(const vector<T>&v0, vector<T>&v1) {
	for_each(begin(v0), end(v0), [&v1](auto &e) {
		v1.emplace_back(e);
	});
}



#endif /* SRC_COMMON_UTILITY_HPP_ */
