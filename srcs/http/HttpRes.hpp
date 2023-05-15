#ifndef HTTPRES_HPP
#define HTTPRES_HPP

#include <string>
#include <sstream>
#include <vector>
#include "../conf/Location.hpp"
#include "../conf/virtualServer.hpp"
#include "httpReq.hpp"
#include "../Client.hpp"

class HttpRes {
	private:
		std::string head;
		std::string body;
		// request, vserverはclientのをそのまま使うからデータの持ち方どうしよう
		// 親のクライアントへの参照を持つのはあり
		httpReq httpReq;
		virtualServer vServer;
		Location target;
	public:
		HttpRes(const Client& source);
		~HttpRes();
		void longestMatchLocation();
		void read_file();
		void createResponse();
};

#endif
