#ifndef HTTPRES_HPP
#define HTTPRES_HPP

#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>
#include "../conf/Location.hpp"
#include "../conf/virtualServer.hpp"
#include "httpReq.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../Kqueue.hpp"

enum Status {
	OK = 200,
	CREATED = 201,
	NO_CONTENT =204,
	PARTIAL_CONTENT = 206,

	SPECIAL_RESPONSE = 300,
	MOVED_PERMANENTLY = 301,
	MOVED_TEMPORARILY = 302,
	NOT_MODIFIED = 304,

	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	NOT_ALLOWED = 405,
	REQUEST_TIME_OUT = 408,
	CONFLICT = 409,
	LENGTH_REQUIRED = 411,
	PRECONDITION_FAILED = 412,
	REQUEST_ENTITY_TOO_LARGE = 413,
	REQUEST_URI_TOO_LARGE = 414,
	UNSUPPORTED_MEDIA_TYPE = 415,
	RANGE_NOT_SATISFIABLE = 416,

	INTERNAL_SERVER_ERROR = 500,
	NGX_HTTP_NOT_IMPLEMENTED = 501,
	NGX_HTTP_BAD_GATEWAY = 502,
	NGX_HTTP_SERVICE_UNAVAILABLE = 503,
	NGX_HTTP_GATEWAY_TIME_OUT = 504,
	NGX_HTTP_INSUFFICIENT_STORAGE = 507
};

class Client;

class HttpRes {
	private:
		static const std::string kServerName;
		std::string header;
		std::string body;

		int status_code;
		std::string status_string;
		std::string status_line;
		size_t content_length_n;
		std::string content_type;
		static const std::string default_type;
		time_t last_modified_time;
//		struct timespec last_modified_time;
//		std::string buf;
		bool is_posted;
		std::string location;
		bool header_only;
        time_t last_modified;
        std::string charset;
//        size_t header_size;

		// 対応可能なMedia-Typeを持つ
		//static const std::map<std::string, std::string> types;// = {{"html", "text/html"},{"json", "application/json"}};
		// request, vserverはclientのをそのまま使うからデータの持ち方どうしよう
		// 親のクライアントへの参照を持つのはあり
//		httpReq httpReq;

		httpReq httpreq;
		virtualServer vServer;
        Kqueue connection;
		int fd;

		Location target;
		void write_file();
		void delete_file();
		void read_file();
		void createResponseHeader(struct stat sb);
		//void createResponseBody();
		std::string getStatusString();
		void createControlData();
        void createDate(time_t now, std::string fieldName);
		void createContentLength();
		void set_content_type();
		void post_event();
		void ev_queue_insert();
		void header_filter();
        void static_handler();
        void sendHeader();
        Location get_uri2location(std::string uri) const;
        //void createDate();
	public:
        HttpRes();
        HttpRes(const HttpRes& src);
		HttpRes(const Client& source, Kqueue kq);
		~HttpRes();
		Location longestMatchLocation(std::string request_path, std::vector<Location> locations);
        bool isAllowMethod(std::string method);
        std::string join_path();
        void set_body(std::string strs);
		void createResponse();
        void runHandlers();
		std::string buf;
        size_t header_size;
        std::string out_buf;
        size_t body_size;
};

#endif
