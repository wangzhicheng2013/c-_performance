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
*  @date     2019-06-05                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/06/03 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*  2019/06/04 | 1.0.0     |                | Add get_next_pose               *
*****************************************************************************/
#ifndef SRC_ZEG_ROBOT_DEFINE_HPP_
#define SRC_ZEG_ROBOT_DEFINE_HPP_
#include <stdint.h>
#include <cmath>
#include <vector>
#include "msgpack.hpp"
#include "common_utility.hpp"
namespace zeg_robot_simulator {
using namespace std;
static const double M_EPS = 0.00000001;
static const double M_PI_ = 3.1415926535898;
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
	bool operator == (const robot_pose &other) {
		return (x == other.x) && (y == other.y) && (theta == other.theta);
	}
	friend bool operator == (const robot_pose &one, const robot_pose &other) {
		return (one.x == other.x) && (one.y == other.y) && (one.theta == other.theta);
	}
	inline void normalize() {
		theta = atan2(sin(theta), cos(theta));
    }
	inline void set_zero() {
		x = 0;
		y = 0;
		theta = 0;
	}
    double x, y;
    double theta;
	MSGPACK_DEFINE(x, y, theta);
};
inline void add(const robot_pose &p0, const robot_pose &p1, robot_pose &p) {
	p.x = p0.x + p1.x;
	p.y = p0.y + p1.y;
	p.theta = p0.theta + p1.theta;
}
inline double direction(const robot_pose &src_pose, const robot_pose &target_pose) {
	return atan2(target_pose.y - src_pose.y, target_pose.x - src_pose.x);
}
double normalize(double angle) {
	if (fabs(angle) > M_PI_ * 2) {
		angle = angle - long(angle / M_PI_ * 2) * M_PI_ * 2;
	}
	while (angle > M_PI_) {
		angle -= M_PI * 2;
	}
	while (angle <= -M_PI_) {
		angle += M_PI_ * 2;
	}
	return angle;
}
void absolute_sum(const robot_pose &p0, const robot_pose &p1, robot_pose &p) {
	double s = sin(p0.theta);
	double c = cos(p0.theta);
	p.x = c * p1.x - s * p1.y;
	p.y = s * p1.x + c * p1.y;
	p.theta = p1.theta;
	add(p, p0, p);
    p.normalize();
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
	bool operator == (const robot_speed &other) {
		return (vx == other.vx) && (vy == other.vy) && (w == other.w);
	}
	friend bool operator == (const robot_speed &one, const robot_speed &other) {
		return (one.vx == other.vx) && (one.vy == other.vy) && (one.w == other.w);
	}
	double vx, vy, w;
	MSGPACK_DEFINE(vx, vy, w);
};
inline double distance(const robot_pose &a, const robot_pose &b) {
	return hypot(a.x - b.x, a.y - b.y);
}
class pose_compute {
public:
	pose_compute() {
		msecs = 100;
		theta = 0;
	}
	pose_compute(int msecs,
			const robot_pose &cur_pose,
			const robot_pose &destination_pose,
			const robot_speed &speed) : msecs(msecs),
					cur_pose_(cur_pose), destination_pose_(destination_pose), speed_(speed) {
		theta = 0;
	}
	pose_compute(const pose_compute &) = delete;
	pose_compute & operator = (const pose_compute &) = delete;
	virtual ~pose_compute() = default;
public:
	void get_next_pose (robot_pose &next_pose) {
		 robot_pose motion;
		 double v = speed_.vx;
		 double w = deg2rad(speed_.w);
		 if (!equal(speed_.w, 0.0) && !equal(v, 0.0)) {
			 double r = fabs(v / w);
			 double theta = fabs(w * msecs / 1000.0);
			 motion.x = sin(theta) * r * sign(v);
			 motion.y = ((1 - cos(theta)) * r) * sign(v) * sign(w);
			 motion.theta = theta * sign(w);
		 }
		 else if (equal(speed_.w, 0.0)) {
			 motion.x = v * msecs / 1000.0;
		 }
		 else {
			 motion.theta = w * msecs / 1000.0;
		 }
		 absolute_sum(cur_pose_, motion, next_pose);
	}
	bool get_pose_trace(vector<robot_pose>&pose_trace) {
		pose_trace.clear();
		double s = speed_.vx * msecs / 1000;
		double d = distance(cur_pose_, destination_pose_);
		int loop = ceil(d / (msecs / 1000.0));
		int count = 0;
		robot_pose next_pose;
		while (d > s) {
			get_next_pose(next_pose);
			pose_trace.emplace_back(next_pose);
			cur_pose_ = next_pose;
			d = distance(cur_pose_, destination_pose_);
			if (++count > loop) {
				return false;
			}
		}
		return true;
	}
	bool adjust_pose_angle() {
		if (speed_.w <= 0) {
			return false;
		}
		theta = direction(cur_pose_, destination_pose_);
		double normalized_value = normalize(theta - cur_pose_.theta);
		if (fabs(normalized_value) <= speed_.w * msecs / 1000) {
			cur_pose_.theta = theta;
			return false;
		}
		return true;
	}
	inline void rotate_robot_adjust() {
		destination_pose_.x = cur_pose_.x;
		destination_pose_.y = cur_pose_.y;
		destination_pose_.theta = theta;
		speed_.vx = 0;
	}
	void rotate_robot_pose(vector<robot_pose>&pose_trace) {
		pose_trace.clear();
		if (false == adjust_pose_angle()) {
			return;
		}
		double tmp = theta;
		robot_pose save_destination_pose = destination_pose_;
		robot_speed save_speed = speed_;
		rotate_robot_adjust();
		robot_pose next_pose;
		do {
			get_next_pose(next_pose);
			pose_trace.emplace_back(next_pose);
			cur_pose_ = next_pose;
		}while (true == adjust_pose_angle());
		destination_pose_ = save_destination_pose;
		speed_ = save_speed;
		cur_pose_.theta = tmp;
	}
	bool get_pose_trace_with_angle(const vector<robot_pose>&pose_set, vector<robot_pose>&pose_trace) {
		vector<robot_pose>pose_tmp;
		pose_trace.clear();
		for (auto &pose : pose_set) {
			destination_pose_ = pose;
			rotate_robot_pose(pose_tmp);
			merge_vector(pose_tmp, pose_trace);
			if (false == get_pose_trace(pose_tmp)) {
				return false;
			}
			merge_vector(pose_tmp, pose_trace);
			pose_trace.emplace_back(destination_pose_);
		}
		return true;
	}
public:
	int msecs;				// ms
	double theta;
	robot_pose cur_pose_;
	robot_pose destination_pose_;
	robot_speed speed_;
};
}

#endif /* SRC_ZEG_ROBOT_DEFINE_HPP_ */
