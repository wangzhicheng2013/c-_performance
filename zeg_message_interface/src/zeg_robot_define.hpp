#ifndef SRC_ZEG_ROBOT_DEFINE_H_
#define SRC_ZEG_ROBOT_DEFINE_H_
#include <stdint.h>
#include <string>
#include <vector>
#include "msgpack.hpp"
namespace zeg_message_interface {
using namespace std;
enum {
	NAVIGATION_COMMAND,
	LOCALIZATION_COMMAND
};
struct zeg_robot_header {
	zeg_robot_header() {
		timestamp = 0;
	}
	zeg_robot_header(const string &a, const string &b, uint64_t c) :
		type(a),
		robot_id(b),
		timestamp(c) {

	}
	string type;				// zeg.robot
	string robot_id;
	uint64_t timestamp;
	MSGPACK_DEFINE(type, robot_id, timestamp);
};
struct zeg_robot_point {
	zeg_robot_point() {
		x = y = 0;
	}
	zeg_robot_point(double a, double b) :
		x(a),
		y(b) {
	}
	double x, y;
	MSGPACK_DEFINE(x, y);
};
struct zeg_robot_navigation_command {
	string task_id;
	vector<zeg_robot_point>points;
	MSGPACK_DEFINE(task_id, points);
};
struct zeg_robot_navigation_command_ack {
	string task_id;
	MSGPACK_DEFINE(task_id);
};
struct zeg_command_unpack_struct {
	zeg_command_unpack_struct(msgpack::unpacked *a,
			const char *b,
			const zeg_robot_header *c,
			int d,
			size_t e) : unpack_msg(a),
					unpack_buf(b),
					unpack_header(c),
					unpack_len(d),
					unpack_offset(e) {

	}
	msgpack::unpacked *unpack_msg;
	const char *unpack_buf;
	const zeg_robot_header *unpack_header;
	int unpack_len;
	size_t unpack_offset;
};
struct zeg_robot_navigation_lock_point {
	string task_id;
	vector<zeg_robot_point>locked_points;
	MSGPACK_DEFINE(task_id, locked_points);
};
struct zeg_robot_navigation_lock_point_ack {
	string task_id;
	vector<zeg_robot_point>locked_points;
	MSGPACK_DEFINE(task_id, locked_points);
};
}

#endif /* SRC_ZEG_ROBOT_DEFINE_H_ */
