#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <iostream>

class Kqueue {
	private:
		// 仮置き10
		struct kevent register_event[10], reciver_event[10];
		int kq;
		struct timespec time_over;
	public:
		Kqueue();
		~Kqueue();
		struct kevent* get_reciver_event();
		int get_events_num();
		int get_kq();
		void set_event(int fd, short ev_filter);
        void disable_event(int fd, short ev_filter);
//		void set_register_event(int fd);
};

#endif
