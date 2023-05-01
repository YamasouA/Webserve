#include "Socket.hpp"
#include <iostream>


int main(int argc, char *argv[]) {
	// 設定ファイルを読み込む
	// read_config();
	(void)argc;
	(void)argv;
	std::string default_file = "index.html";
	Socket *socket = new Socket(5000); //port num is tmp
	socket->set_socket();
	int acceptfd;
	ssize_t ret;
	char buf[1024];

	struct kevent register_event[4], reciver_event[4];
	int kq = kqueue();
	EV_SET(register_event, socket->get_listenfd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
		perror("kevent");
		std::exit(1);
	}

	while (1) {
		int events_num = kevent(kq, NULL, 0, reciver_event, 1, NULL);
		if (events_num <= 0) {
			perror("kevent");
			std::exit(1);
		}
		for (size_t i = 0; i < events_num; ++i) {
			int event_fd = reciver_event[i].ident;
			if (event[i].flags & EV_EOF) {
				std::cout << "Client has disconnected" << std::endl;
				std::close(event_fd);
			}
			else if (event_fd == socket->get_listenfd()) {
				acceptfd = accept(socket->get_listenfd(), NULL, NULL);
				if (acceptfd == -1) {
					std::cerr << "Accept socket error" << std::endl;
					continue;
				}
				EV_SET(register_event, acceptfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
				if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
					perror("kevent error");
				}
			}
		}
		memset(buf, 0, sizeof(buf));
		ret = read(acceptfd, buf, 1024);
		std::cout << ret << std::endl;
		std::cout << buf << std::endl;
	}
}
