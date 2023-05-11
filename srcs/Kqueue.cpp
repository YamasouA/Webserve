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

void Kqueue::set_event(int fd) {
	EV_SET(register_event, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
		std::cout << "Error kevent" << std::endl;
		exit(1);
		// new throw;
	}
}

void Kqueue::set_register_event(int fd) {
	EV_SET(register_event, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
		perror("kevent error");
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
