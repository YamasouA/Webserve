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
#include "Client.hpp"
#include "conf/configParser.hpp"
//#include "http/httpParser.hpp"
#include "http/httpReq.hpp"
#include <map>
#include <utility>

//std::map<int, virtualServer> initialize_fd(configParser conf, Kqueue kqueue) {
void initialize_fd(configParser conf, Kqueue kqueue, std::map<int, virtualServer>& fd_config_map) {
	std::vector<virtualServer> server_confs = conf.get_serve_confs();
	//std::map<int, virtualServer> fd_config_map;
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
	//return fd_config_map;
}

void assign_server(configParser& conf, Client& client) {
//    std::cout << "ok" << std::endl;
	std::vector<virtualServer> server_confs = conf.get_serve_confs();
	for (std::vector<virtualServer>::iterator it = server_confs.begin();
		it != server_confs.end(); it++) {

        std::map<std::string, std::string> tmp = client.get_httpReq().getHeaderFields();
        std::string host_name;
		for (std::map<std::string, std::string>::iterator req_it = tmp.begin(); req_it != tmp.end(); ++it) {
//            std::cout << "field name: " << (*req_it).getName() << std::endl;
            if ((*req_it).first == "Host") {
                host_name = (*req_it).second;
                break;
            }
        }
//		if (client.get_httpReq().get_hostname() == it->get_server_name()
        std::cout << "server name: " << it->get_server_name() << std::endl;
        if (host_name == it->get_server_name()) {
//			&& client.get_fd() == it->get_listen()) {
			client.set_vServer(*it);

		}
	}
}

void read_request(int fd, Client& client, configParser& conf) {
	char buf[1024];

	memset(buf, 0, sizeof(buf));
	fcntl(fd, F_SETFL, O_NONBLOCK);
	if (recv(fd, buf, sizeof(buf), 0) < 0) {
        return ;
    }
	/*
	if (buf ==) {
		std::cout << "ERROR" << std::endl;

	}
	*/
	//client.get_httpReq(buf)->parserRequest();

    std::cout << "req: " << buf << std::endl;
    //httpParser httpparser(buf);
    httpReq httpreq(buf);
    httpreq.parseRequest();
    client.set_httpReq(httpreq);
//	client.set_httpReq(httpparser.get);
    assign_server(conf, client);
    HttpRes respons(client);
    respons.createResponse();

}

int main(int argc, char *argv[]) {
	// 設定ファイルを読み込む
	// read_config();
	(void)argc;
	(void)argv;
	std::map<int, virtualServer> fd_config_map;
	std::map<int, Client> fd_client_map;

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
		//fd_config_map = initialize_fd(conf, kqueue);
		initialize_fd(conf, kqueue, fd_config_map);
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
				Client client;
				acceptfd = accept(event_fd, NULL, NULL);
				if (acceptfd == -1) {
					std::cerr << "Accept socket error" << std::endl;
					continue;
				}
				std::cout << "client fd: " << acceptfd << std::endl;
				kqueue.set_register_event(acceptfd);
				client.set_fd(acceptfd);
				fd_client_map.insert(std::make_pair(acceptfd, client));
				/*
				EV_SET(register_event, acceptfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, register_event, 1, NULL, 0, NULL) == -1) {
					perror("kevent error");
				}
				*/
			} else if (reciver_event[i].filter & EVFILT_READ) {
				char buf[1024];
				memset(buf, 0, sizeof(buf));
//				fcntl(event_fd, F_SETFL, O_NONBLOCK);
				//fcntl(0, F_SETFL, O_NONBLOCK);
				//read(0, buf, 1);
				//recv(event_fd, buf, sizeof(buf), 0);
				//client->set_request(buf);
				read_request(event_fd, fd_client_map[acceptfd], conf);
//				std::cout << buf << std::endl;
//				std::cout << "ok" << std::endl;
			}
		}
	}
}
