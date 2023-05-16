#ifndef HTTPRES_HPP
#define HTTPRES_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "../conf/Location.hpp"
#include "../conf/virtualServer.hpp"
#include "httpReq.hpp"

class Client;

class HttpRes {
	private:
		std::string head;
		std::string body;
		// request, vserverはclientのをそのまま使うからデータの持ち方どうしよう
		// 親のクライアントへの参照を持つのはあり
//		httpReq httpReq;
		httpReq httpreq;
		virtualServer vServer;
		Location target;
	public:
		HttpRes(const Client& source);
		~HttpRes();
		Location longestMatchLocation(std::string request_path, std::vector<Location> locations);
        bool isAllowMethod(std::string method);
        std::string join_path();
        void set_body(std::string strs);
		void read_file();
		void createResponse();
};

#endif
