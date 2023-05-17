#ifndef HTTPRES_HPP
#define HTTPRES_HPP

#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "../conf/Location.hpp"
#include "../conf/virtualServer.hpp"
#include "httpReq.hpp"
#include <sys/types.h>
#include <sys/stat.h>


class Client;

class HttpRes {
	private:
		std::string head;
		std::string body;

		int status_code;
		std::string status_string;
		// request, vserverはclientのをそのまま使うからデータの持ち方どうしよう
		// 親のクライアントへの参照を持つのはあり
//		httpReq httpReq;
		httpReq httpreq;
		virtualServer vServer;
		Location target;
		void write_file();
		void delete_file();
		void read_file();
		void createResponseBody();
		std::string getStatusString();
		void createControlData();
	public:
		HttpRes(const Client& source);
		~HttpRes();
		Location longestMatchLocation(std::string request_path, std::vector<Location> locations);
        bool isAllowMethod(std::string method);
        std::string join_path();
        void set_body(std::string strs);
		void createResponse();
};

#endif
