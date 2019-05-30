/*****************************************************************************
*  zeg mock_navigate_server                                                  *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     zeg_mock_navigate_server.hpp                                    *
*  @brief    zeg mock navigate command server                                *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-05-30                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/05/30 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*                                                                   *
*****************************************************************************/
#ifndef SRC_ZEG_MOCK_NAVIGATE_SERVER_HPP_
#define SRC_ZEG_MOCK_NAVIGATE_SERVER_HPP_
#include "zeg_config.hpp"
#include "rpc_server.h"
namespace zeg_message_interface {
using namespace rest_rpc;
using namespace rpc_service;

class zeg_mock_navigate_server {
public:
	std::string get_person_name(rpc_conn conn, const person& p) {
	        return p.name;
	}
	std::string get_person_name(rpc_conn conn, const person& p) {
	        return p.name;
	}

};
}

#endif /* SRC_ZEG_MOCK_NAVIGATE_SERVER_HPP_ */
