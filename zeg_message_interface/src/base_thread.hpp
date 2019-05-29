/*****************************************************************************
*  base thread class                                                         *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     base_thread.hpp                                                 *
*  @brief    base thread                                                     *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-05-29                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/05/29 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*                                                                   *
*****************************************************************************/
#ifndef SRC_BASE_THREAD_HPP_
#define SRC_BASE_THREAD_HPP_
#include <thread>
using namespace std;
class base_thread {
public:
	base_thread() = default;
	virtual ~base_thread() = default;
public:
	void run() {
		thd_ = thread([this] {
					this->todo();
			});
	}
	void join() {
		if (thd_.joinable()) {
			thd_.join();
		}
	}
protected:
	virtual void todo() = 0;
private:
	thread thd_;
};
#endif /* SRC_BASE_THREAD_HPP_ */
