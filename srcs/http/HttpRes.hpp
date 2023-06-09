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
#include <dirent.h>
#include "../Kqueue.hpp"

enum server_state {
    OK = 0,
    DECLINED = -1 //tmp value
};

enum Status {
	HTTP_OK = 200,
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
	HTTP_NOT_IMPLEMENTED = 501,
	HTTP_BAD_GATEWAY = 502,
	HTTP_SERVICE_UNAVAILABLE = 503,
	HTTP_GATEWAY_TIME_OUT = 504,
	HTTP_INSUFFICIENT_STORAGE = 507
};

typedef struct {
    DIR             *dir;
    struct dirent   *d_ent;
    struct stat     d_info;
    bool            valid_info:1;
} dir_t;

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
        int keep_alive;

        int err_status;
//        size_t header_size;

		// 対応可能なMedia-Typeを持つ
		//static const std::map<std::string, std::string> types;// = {{"html", "text/html"},{"json", "application/json"}};
		// request, vserverはclientのをそのまま使うからデータの持ち方どうしよう
		// 親のクライアントへの参照を持つのはあり
//		httpReq httpReq;

		httpReq httpreq;
		virtualServer vServer;
        Kqueue* connection;
		int fd;

		Location target;
		void write_file();
		void delete_file();
		void read_file();
		void createResponseHeader(struct stat sb);
		//void createResponseBody();
		std::string getStatusString();
		void createControlData();
        std::string createDate(time_t now, std::string fieldName);
//        void createDate(time_t now, std::string fieldName);
		void createContentLength();
		void set_content_type();
		void post_event();
		void ev_queue_insert();
		void header_filter();
        int static_handler();
        void sendHeader();
        Location get_uri2location(std::string uri) const;

		void dav_delete_handler();
		void dav_delete_path(bool is_dir);
		int dav_depth();
        std::string join_dir_path(const std::string& dir_path, const std::string& elem_name);
        void diving_through_dir(const std::string& path);
        void finalize_res(int handler_status);
        std::string create_err_page();
        int redirect_handler();
		int return_redirect();
        int send_error_page();
        int auto_index_handler();
        std::string create_auto_index_html(std::map<std::string, dir_t> index_of);
        std::string join_path_autoindex();

        //void createDate();
	public:
        HttpRes();
        HttpRes(const HttpRes& src);
		HttpRes(const Client& source, Kqueue &kq);
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
		std::string redirect_path;
};

#endif
