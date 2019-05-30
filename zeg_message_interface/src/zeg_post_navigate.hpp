/*****************************************************************************
*  zeg post navigate                                                         *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     zeg_post_navigate.hpp                                           *
*  @brief    zeg post navigate command                                       *
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
#ifndef SRC_ZEG_POST_NAVIGATE_HPP_
#define SRC_ZEG_POST_NAVIGATE_HPP_
#include <exception>
#include "base_thread.hpp"
#include "zeg_config.hpp"
namespace zeg_message_interface {
class zeg_post_navigate : public base_thread {
protected:
	virtual void todo() override {
		string cmd_str;
		while (true) {
			zeg_config::get_instance().navigate_cmd_queue.wait_dequeue(cmd_str);
			znavigate_command cmd = {0};
			if (false == unpack_command(cmd_str, cmd)) {
				continue;
			}
		}
	}
private:
	bool unpack_command(const string &cmd_str, znavigate_command &cmd) {
		msgpack::unpacked msg;
		msgpack::unpack(&msg, cmd_str.c_str(), cmd_str.size());
		msgpack::object obj = msg.get();
		try {
			obj.convert(&cmd);
		}
		catch (exception &e) {
			LOG_CRIT << e.what();
			return false;
		}
		LOG_INFO << "navigate command taskid = " << cmd.task_id;
		return true;
	}
public:
	uint64_t test_unpack_command() {
		string cmd_str;
		if (false == zeg_config::get_instance().navigate_cmd_queue.try_dequeue(cmd_str)) {
			return 0;
		}
		znavigate_command cmd = {0};
		if (false == unpack_command(cmd_str, cmd)) {
			return 0;
		}
		return cmd.task_id;
	}
};
}

#endif /* SRC_ZEG_POST_NAVIGATE_HPP_ */
