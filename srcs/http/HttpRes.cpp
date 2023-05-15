#include "HttpRes.hpp"

HttpRes::HttpRes(const Client& source) {
	this->httpReq = source.get_httpReq();
	this->vServer = source.get_vServer();
}

HttpRes::~HttpRes() {
}

Location HttpRes::longestMatchLocation(std::string request_path, std::vector<Location> locations) {
	Location location;
	size_t max_len = 0;
	for (std::vector<Location>::iterator it = locations.begin(); it!= locations.end(); it++) {
		location_path = it->get_uri();
		// locationの方が長い場合はマッチしない
		if (request_path.find(location_path) == 0) {
			// 終端が'/'でないまたは、最後のスペルまでマッチしている以外は
			// 中途半端にマッチしていて間違っている
			if (request_path[location_path.length()] == '/' || request_path.length() == it->get_path().length()) {
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
	std::vector<std::string> allow_method = target->get_methods();
	for (std::vector<std::string>::iterator it = allow_method.begin();
		it != allow_method.end(); it++) {
		if (*it == method) {
			return true;
		}
	}
	return false;
}

std::string HttpRes::join_path() {
	std::string filename = vServer.get_uri();
	std::string file_path  = target.get_uri();
	if (filename == "" && vServer.get_autoindex()) {
		filename = "index.html";
	}
	return file_path + filename;
}

void HttpRes::read_file() {
	std::string buf[1024];
	
	// locationから正しいファイルパスを作成する
	std::string file_name = join_path();
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
	target = longestMatchLocation(httpreq.get_location(), vServer.get_locations());
	std::string method = httpReq.getMethod();
	if (isAllowMethod(method)) {
		if (method == "GET") {
			read_file();
		}
	}
}
