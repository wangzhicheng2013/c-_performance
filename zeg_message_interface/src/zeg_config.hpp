/*****************************************************************************
*  zeg configuration entry                                                   *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     zeg_config.hpp                                                  *
*  @brief    zeg configuration entry                                         *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-05-28                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/05/27 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*                                                                   *
*****************************************************************************/
#ifndef SRC_ZEG_CONFIG_HPP_
#define SRC_ZEG_CONFIG_HPP_
#include <string>
namespace zeg_message_interface {
using namespace std;
class zeg_config {
private:
	constexpr zeg_config() = default;
	virtual ~zeg_config() = default;
public:
	inline static zeg_config &get_instance() {
		return config_;
	}
public:
	static zeg_config config_;
public:
	const char *g_server_address = "tcp://localhost:9141";
	const char *g_local_address = "tcp://*:9142";
};
zeg_config zeg_config::config_;
}

#endif /* SRC_ZEG_CONFIG_HPP_ */
