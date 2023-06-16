#include "Kqueue.hpp"

Kqueue::Kqueue() {
	kq = kqueue();
	if (kq < -1) {
		std::cerr << "kqueue Error" << std::endl;
		exit(1);
		// new throw();
	}
	time_over.tv_sec = 10;
	time_over.tv_nsec = 0;
}

Kqueue::~Kqueue() {}

//void Kqueue::set_event(int fd) {
//	EV_SET(register_event, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
//	if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
//		std::cout << "Error kevent" << std::endl;
//		exit(1);
//		// new throw;
//	}
//}

void Kqueue::set_event(int fd, short ev_filter) {
	if (ev_filter == EVFILT_READ) {
		std::cout << "hoge1" << std::endl;
		EV_SET(&register_event[0], fd, ev_filter, EV_ADD | EV_ENABLE, 0, 0, NULL);
		if (kevent(kq, &register_event[0], 1, NULL, 0, NULL) == -1) {
			perror("kevent error");
		}
	}
	if (ev_filter == EVFILT_WRITE) {
		std::cout << "hoge2" << std::endl;
		EV_SET(&register_event[1], fd, ev_filter, EV_ADD | EV_ENABLE, 0, 0, NULL);
		if (kevent(kq, &register_event[1], 1, NULL, 0, NULL) == -1) {
			perror("kevent error");
		}
	}
}

void Kqueue::disable_event(int fd, short ev_filter) {
	//EV_SET(&register_event[1], fd, ev_filter, EV_DISABLE, 0, 0, NULL);

	if (ev_filter == EVFILT_READ) {
		std::cout << "hoge3" << std::endl;
		EV_SET(&register_event[2], fd, ev_filter, EV_DELETE, 0, 0, NULL);
		if (kevent(kq, &register_event[2], 1, NULL, 0, NULL) == -1) {
			perror("kevent error");
    	}
	}
	if (ev_filter == EVFILT_WRITE) {
		std::cout << "hoge2" << std::endl;
		EV_SET(&register_event[3], fd, ev_filter, EV_DELETE, 0, 0, NULL);
		if (kevent(kq, &register_event[3], 1, NULL, 0, NULL) == -1) {
			perror("kevent error");
    	}
	}
}

int Kqueue::get_kq() {
	return kq;
}

int Kqueue::get_events_num() {
	return kevent(kq, NULL, 0, reciver_event, 1, &time_over);
}

struct kevent* Kqueue::get_reciver_event() {
	return reciver_event;
}
