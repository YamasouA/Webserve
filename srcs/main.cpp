#include "Socket.hpp"
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include "Logger.hpp"


int main(int argc, char *argv[]) {
	// 設定ファイルを読み込む
	// read_config();
	(void)argc;
	(void)argv;

	if (argc != 1 && argc != 2) {
		std::cout << "usage: ./webserv *(path_to_config_file)" << std::endl;
		return 1;
	}
	std::string config_path = (argc == 1? "conf/default.conf": argv[1]);
	/*
	try {
		//contents = readConfFile();
		configParser conf = configParser(readConfFile());
		conf.parse();
	} catch (const std::exception &e) {
		
	}*/
	std::string default_file = "index.html";
	Socket *socket = new Socket(8000); //port num is tmp
	socket->set_socket();

	int acceptfd;
	//ssize_t ret;
	//char buf[1024];

	struct timespec time_over;
	time_over.tv_sec = 10;
	time_over.tv_nsec = 0;

	struct kevent register_event[4], reciver_event[4];
	int kq = kqueue();
	EV_SET(register_event, socket->get_listenfd(), EVFILT_READ, EV_ADD | EV_ENABLE , 0, 0, NULL);
	if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
		perror("kevent");
		std::exit(1);
	}

	while (1) {
		Logger::logging("hello");
		int events_num = kevent(kq, NULL, 0, reciver_event, 1, &time_over);
		if (events_num == -1) {
			perror("kevent");
			std::exit(1);
		} else if (events_num == 0) {
			std::cout << "time over" << std::endl;
			continue;
		}

		for (int i = 0; i < events_num; ++i) {
			int event_fd = reciver_event[i].ident;
			if (reciver_event[i].flags & EV_EOF) {
				std::cout << "Client " << event_fd << " has disconnected" << std::endl;
				close(event_fd);
			}
			else if (event_fd == socket->get_listenfd()) {
				acceptfd = accept(socket->get_listenfd(), NULL, NULL);
				if (acceptfd == -1) {
					std::cerr << "Accept socket error" << std::endl;
					continue;
				}
				std::cout << "client fd: " << acceptfd << std::endl;
				EV_SET(register_event, acceptfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
					perror("kevent error");
				}
			} else if (reciver_event[i].filter & EVFILT_READ) {
				char buf[1024];
				memset(buf, 0, sizeof(buf));
				fcntl(event_fd, F_SETFL, O_NONBLOCK);
				//fcntl(0, F_SETFL, O_NONBLOCK);
				//read(0, buf, 1);
				recv(event_fd, buf, sizeof(buf), 0);
				std::cout << buf << std::endl;
				std::cout << "ok" << std::endl;
			}
		}
	}
}
