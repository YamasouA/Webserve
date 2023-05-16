#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "conf/virtualServer.hpp"
#include "./http/HttpRes.hpp"
#include "./http/httpReq.hpp"

class Client {
	private:
		httpReq httpreq;
		HttpRes httpres;
		virtualServer vServer;
		int fd;
	public:
		Client();
		~Client();
		Client(const Client& source);
		Client& operator =(const Client& source);

		void set_fd(int fd);
		void set_httpReq(const httpReq& source);
		void set_httpRes(const HttpRes& source);
		void set_server(const virtualServer& source);
		int get_fd();
		httpReq get_httpReq();
		HttpRes get_httpRes();
		virtualServer get_vServer();
};

#endif
