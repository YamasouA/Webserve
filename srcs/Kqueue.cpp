#include "Kqueue.hpp"

Kqueue::Kqueue()
:reciver_event(new struct kevent)
{
	kq = kqueue();
	if (kq < -1) {
		std::cerr << "kqueue Error" << std::endl;
		exit(1);
		// new throw();
	}
	time_over.tv_sec = 10;
	time_over.tv_nsec = 0;
}

Kqueue::Kqueue(const Kqueue& src) {
    this->changes = src.changes;
    this->reciver_event = src.reciver_event;
    this->kq = src.kq;
    this->time_over = src.time_over;
}

Kqueue& Kqueue::operator=(const Kqueue& rhs) {
    if (this == &rhs) {
        return *this;
    }
    this->changes = rhs.changes;
    this->reciver_event = rhs.reciver_event;
    this->kq = rhs.kq;
    this->time_over = rhs.time_over;
    return *this;
}

Kqueue::~Kqueue() {
    delete reciver_event;
}



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
		std::cout << "READ register" << std::endl;
	}
	if (ev_filter == EVFILT_WRITE) {
		std::cout << "WRITE register" << std::endl;
	}
	struct kevent register_event;
	EV_SET(&register_event, fd, ev_filter, EV_ADD | EV_ENABLE, 0, 0, NULL);
	changes.push_back(register_event);
	if (kevent(kq, &register_event, 1, NULL, 0, NULL) == -1) {
		perror("kevent error");
    }
	std::cout << "REGISTER: changes.size(): " << changes.size() << std::endl;
}

void Kqueue::disable_event(int fd, short ev_filter) {
	if (ev_filter == EVFILT_READ) {
		std::cout << "READ delete" << std::endl;
	}
	if (ev_filter == EVFILT_WRITE) {
		std::cout << "WRITE delete" << std::endl;
	}
	struct kevent register_event;
	EV_SET(&register_event, fd, ev_filter, EV_DELETE, 0, 0, NULL);
	if (kevent(kq, &register_event, 1, NULL, 0, NULL) == -1) {
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
