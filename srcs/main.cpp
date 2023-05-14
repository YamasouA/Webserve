#include "Socket.hpp"
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include "Logger.hpp"
#include "Kqueue.hpp"
#include "conf/configParser.hpp"
#include <map>

//void initialize_fd(configParser conf, Kqueue kqueue) {
std::map<int, virtualServer> initialize_fd(configParser conf, Kqueue kqueue) {
	std::vector<virtualServer> server_confs = conf.get_serve_confs();
	std::map<int, virtualServer> fd_config_map;
	for (std::vector<virtualServer>::iterator it = server_confs.begin(); it != server_confs.end(); it++) {
		//std::cout << "=============" << std::endl;
		//std::cout << *it << std::endl;
		//std::string default_file = *it.get_root();
		std::cout << "it->get_listen(): " << it->get_listen() << std::endl;
		Socket *socket = new Socket(it->get_listen());
		//Socket *socket = new Socket(8000);
		socket->set_socket();
		kqueue.set_event(socket->get_listenfd());
		fd_config_map[socket->get_listenfd()] = *it;
	}
	return fd_config_map;
}

void readRequest(int event_fd) {
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	fcntl(event_fd, F_SETFL, O_NONBLOCK);
	recv(event_fd, buf, sizeof(buf), 0);
	std::cout << "buf: " << std::endl;
	std::cout << buf << std::endl;
	
}

int main(int argc, char *argv[]) {
	// 設定ファイルを読み込む
	// read_config();
	(void)argc;
	(void)argv;
	std::map<int, virtualServer> fd_config_map;

	if (argc != 1 && argc != 2) {
		std::cout << "usage: ./webserv *(path_to_config_file)" << std::endl;
		return 1;
	}
	std::string config_path = (argc == 1? "conf/valid_test/tmp.conf": argv[1]);
	//try {
		//contents = readConfFile();
		std::string txt= readConfFile(config_path);
		configParser conf(txt);
		conf.parseConf();
		//std::cout << conf.get_serve_confs()[0] << std::endl;
		Kqueue kqueue;
		fd_config_map = initialize_fd(conf, kqueue);
	//} catch (const std::exception &e) {
		
	//}
	//std::string default_file = "index.html";
	//Socket *socket = new Socket(8000); //port num is tmp
	//socket->set_socket();

	int acceptfd;
	//ssize_t ret;
	//char buf[1024];

	struct timespec time_over;
	time_over.tv_sec = 10;
	time_over.tv_nsec = 0;

	/*
	struct kevent register_event[4], reciver_event[4];
	int kq = kqueue();
	EV_SET(register_event, socket->get_listenfd(), EVFILT_READ, EV_ADD | EV_ENABLE , 0, 0, NULL);
	if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
		perror("kevent");
		std::exit(1);
	}
	*/
	
	while (1) {
		Logger::logging("hello");
		//int events_num = kevent(kqueue->get_kq(), NULL, 0, reciver_event, 1, &time_over);
		int events_num = kqueue.get_events_num();
		if (events_num == -1) {
			perror("kevent");
			std::exit(1);
		} else if (events_num == 0) {
			std::cout << "time over" << std::endl;
			continue;
		}

		for (int i = 0; i < events_num; ++i) {
			//int event_fd = reciver_event[i].ident;
			struct kevent* reciver_event = kqueue.get_reciver_event();
			int event_fd = reciver_event[i].ident;;
			if (reciver_event[i].flags & EV_EOF) {
				std::cout << "Client " << event_fd << " has disconnected" << std::endl;
				close(event_fd);
			}
			//else if (event_fd == socket->get_listenfd()) {
			else if (fd_config_map.count(event_fd) == 1) {
				//acceptfd = accept(socket->get_listenfd(), NULL, NULL);
				acceptfd = accept(event_fd, NULL, NULL);
				if (acceptfd == -1) {
					std::cerr << "Accept socket error" << std::endl;
					continue;
				}
				std::cout << "client fd: " << acceptfd << std::endl;
				kqueue.set_register_event(acceptfd);
				/*
				EV_SET(register_event, acceptfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
					perror("kevent error");
				}
				*/
			} else if (reciver_event[i].filter & EVFILT_READ) {
				//char buf[1024];
				//memset(buf, 0, sizeof(buf));
				//fcntl(event_fd, F_SETFL, O_NONBLOCK);
				//fcntl(0, F_SETFL, O_NONBLOCK);
				//read(0, buf, 1);
				//recv(event_fd, buf, sizeof(buf), 0);
				readRequest(event_fd);
				//std::cout << buf << std::endl;
				//std::cout << "ok" << std::endl;
			}
		}
	}
}
