#include "Socket.hpp"
#include <iostream>
int main(int argc, char *argv[]) {
	// 設定ファイルを読み込む
	// read_config();

	std::string default_file = "index.html";
	Socket *socket = new Socket();
	socket->set_socket();
	int acceptfd;
	size_t ret;
	char *buf;

	while (1) {
		acceptfd = accept(socket->get_listenfd(), NULL, NULL);
		ret = read(acceptfd, buf, 1024);
		std::cout << buf << std::endl;
	}
}
