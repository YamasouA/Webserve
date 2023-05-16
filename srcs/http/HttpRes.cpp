#include "HttpRes.hpp"
#include "../Client.hpp"

HttpRes::HttpRes(const Client& source) {
	this->httpparser = source.get_parsedReq();
//	this->httpReq = source.get_httpReq();
	this->vServer = source.get_vServer();
}

HttpRes::~HttpRes() {
}

Location HttpRes::longestMatchLocation(std::string request_path, std::vector<Location> locations) {
	Location location;
	size_t max_len = 0;
    size_t i = 0;
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
        std::string location_path = it->get_uri();
		// locationの方が長い場合はマッチしない
		if (request_path.find(location_path) == 0) {
			// 終端が'/'でないまたは、最後のスペルまでマッチしている以外は
			// 中途半端にマッチしていて間違っている
			if (request_path[location_path.length()] == '/' || request_path.length() == location_path.length()) {
				if (location_path.length() > max_len) {
					max_len = location_path.length();
					location = *it;
				}
			}
		}
	}
	return location;
}

bool HttpRes::isAllowMethod(std::string method) {
	std::vector<std::string> allow_method = target.get_methods();
	for (std::vector<std::string>::iterator it = allow_method.begin();
		it != allow_method.end(); it++) {
		if (*it == method) {
			return true;
		}
	}
	return false;
}

std::string HttpRes::join_path() {
	std::string path_root = target.get_root();
//    std::cout << "root: " << path_root << std::endl;
	std::string file_path  = target.get_uri();
//    std::cout << "uri: " << file_path << std::endl;
	if (file_path[file_path.length() - 1] == '/' && target.get_is_autoindex()) {
		file_path = "index.html"; //  "/index" is better?
	}
	return path_root + file_path;
}

void HttpRes::set_body(std::string strs)
{
    this->body = strs;
}

void HttpRes::read_file() {
	std::string buf[1024];

	// locationから正しいファイルパスを作成する
	std::string file_name = join_path();
//    std::cout << file_name << std::endl;
	std::ifstream ifs(file_name.c_str());
	if (!ifs) {
		std::cout << "ko" << std::endl;
	}
	std::ostringstream oss;
	oss << ifs.rdbuf();
	const std::string strs = oss.str();
	set_body(strs);
	std::cout << body << std::endl;
}

void HttpRes::createResponse() {
	// 一つもマッチしない場合は？
	target = longestMatchLocation(httpparser.getUri(), vServer.get_locations());
	std::string method = httpparser.getMethod();
	if (isAllowMethod(method)) {
		if (method == "GET") {
			read_file();
		}
	}
}
