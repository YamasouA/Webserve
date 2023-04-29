#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Socket {
	private:
		int listenfd;
		int port;
		struct sockaddr_in serv_addr;
	public:
		Socket(int port);
		~Socket();
		Socket(const Socket& source);
		Socket& operator =(const Socket& source);

		void set_listenfd();
		void set_port(int port);
		void set_serv_addr();

		int get_listenfd();
		int get_port();
		struct sockaddr_in get_serv_addr();
};
