/*****************************************************************************
*  zeg robot define                                                          *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     zeg_robot_define.hpp                                            *
*  @brief    zeg robot data structure define                                 *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-06-03                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/06/03 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*                                                            *
*****************************************************************************/
#ifndef SRC_ZEG_ROBOT_DEFINE_HPP_
#define SRC_ZEG_ROBOT_DEFINE_HPP_
#include <stdint.h>
#include <cmath>
#include "msgpack.hpp"
namespace zeg_robot_simulator {
using namespace std;
static const double M_EPS = 0.00000001;
static const double DEG2RAD_ZEG = 0.017453292519943;
template <typename T>
inline bool equal(T x, T y) {
        return fabs(x - y) < M_EPS;
}
inline double deg2rad(double angle) {
	return angle * DEG2RAD_ZEG;
}
template <typename T>
inline int sign(T d) {
	return (d >= 0) ? 1 : -1;
}
struct robot_pose {
	inline robot_pose(double x = 0, double y = 0, double theta = 0) : x(x), y(y), theta(theta) {
	}
	robot_pose(const robot_pose &other) {
		x = other.x;
		y = other.y;
		theta = other.theta;
	}
	robot_pose & operator = (const robot_pose &other) {
		x = other.x;
		y = other.y;
		theta = other.theta;
		return *this;
	}
	inline void normalize() {
		theta = atan2(sin(theta), cos(theta));
    }
    double x, y;
    double theta;
	MSGPACK_DEFINE(x, y, theta);
};
robot_pose && operator + (const robot_pose &p0, const robot_pose &p1) {
	return move(robot_pose(p0.x + p1.x, p0.y + p1.y, p0.theta + p1.theta));
}
robot_pose && absolute_sum(const robot_pose &p0, const robot_pose &p1) {
	double s = sin(p0.theta);
	double c = cos(p0.theta);
	robot_pose ans(c * p1.x - s * p1.y, s * p1.x + c * p1.y, p1.theta);
	ans = ans + p0;
    ans.normalize();
    return move(ans);
}
struct robot_speed {
	robot_speed(double vx = 0, double vy = 0, double w = 0) : vx(vx), vy(vy), w(w) {
	}
	robot_speed(const robot_speed &other) {
		vx = other.vx;
		vy = other.vy;
		w = other.w;
	}
	robot_speed & operator = (const robot_speed &other) {
		vx = other.vx;
		vy = other.vy;
		w = other.w;
		return *this;
	}
	double vx, vy, w;
	MSGPACK_DEFINE(vx, vy, w);
};
inline double distance(const robot_pose &a, const robot_pose &b) {
	return hypot(a.x - b.x, a.y - b.y);
}
class pose_compute {
public:
	void operator () () {
		 robot_pose motion;
		 double v = speed.vx;
		 double w = deg2rad(speed.w);
		 if (equal(speed.w, 0.0)) {
			 motion.x = v * msecs / 1000.0;
		 }
		 else if (equal(v, 0.0)) {
			 motion.theta = w * msecs / 1000.0;
		 }
		 else {
			 double r = fabs(v / w);
			 double theta = fabs(w * msecs / 1000.0);
			 motion.x = sin(theta) * r * sign(v);
			 motion.y = ((1 - cos(theta)) * r) * sign(v) * sign(w);
			 motion.theta = theta * sign(w);
		 }
		 next_pose = absolute_sum(cur_pose, motion);
	}
public:
	int msecs;				// ms
	robot_pose cur_pose;
	robot_speed speed;
	robot_pose next_pose;
};
}

#endif /* SRC_ZEG_ROBOT_DEFINE_HPP_ */
