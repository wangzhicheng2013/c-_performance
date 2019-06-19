#ifndef SRC_ZEG_ROBOT_DEFINE_HPP_
#define SRC_ZEG_ROBOT_DEFINE_HPP_
#include <stdint.h>
#include <string>
#include "msgpack.hpp"
namespace zeg_robot_maintainence {
using namespace std;
struct robot_basic_info {
	string protocol;
	double x, y, theta;
	uint32_t state;
	double battery_percentage;
	uint64_t timestamp;
	string vehicle_id;
	MSGPACK_DEFINE(protocol, state, timestamp, x, y, theta, battery_percentage);
};
struct robot_basic_info1 {
	double x, y, theta;
	uint32_t state;
	double battery_percentage;
	MSGPACK_DEFINE(state, x, y, theta, battery_percentage);
};
struct zeg_robot_header {
	string type;
	string robot_id;
	uint64_t timestamp;
	MSGPACK_DEFINE(type, robot_id, timestamp);
};
}

#endif /* SRC_ZEG_ROBOT_DEFINE_HPP_ */
