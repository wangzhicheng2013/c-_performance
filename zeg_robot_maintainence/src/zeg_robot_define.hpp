#ifndef SRC_ZEG_ROBOT_DEFINE_HPP_
#define SRC_ZEG_ROBOT_DEFINE_HPP_
#include <stdint.h>
#include <string>
#include "msgpack.hpp"
namespace zeg_robot_maintainence {
using namespace std;
struct robot_basic_info {
	double x, y, theta;
	uint32_t state;
	double battery_percentage;
	uint64_t timestamp;
	string vehicle_id;
	string protocol;
	MSGPACK_DEFINE(protocol, vehicle_id, state, timestamp, x, y, theta, battery_percentage);
};
}

#endif /* SRC_ZEG_ROBOT_DEFINE_HPP_ */
