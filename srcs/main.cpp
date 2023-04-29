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

	while (1) {
		acceptfd = accept(socket->get_listenfd(), NULL, NULL);
		if (acceptfd == -1)
			continue;
		memset(buf, 0, sizeof(buf));
		ret = read(acceptfd, buf, 1024);
		std::cout << ret << std::endl;
		std::cout << buf << std::endl;
	}
}
