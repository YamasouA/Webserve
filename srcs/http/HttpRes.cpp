#include "HttpRes.hpp"
#include "../Client.hpp"

HttpRes::HttpRes(const Client& source) {
	//this->httpreq = source.get_parsedReq();
	this->httpreq = source.get_httpReq();
	this->vServer = source.get_vServer();
}

HttpRes::~HttpRes() {
}

Location HttpRes::longestMatchLocation(std::string request_path, std::vector<Location> locations) {
	Location location;
	size_t max_len = 0;
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
        std::string location_path = it->get_uri();
		// locationの方が長い場合はマッチしない
		std::cout << "request_path: " << request_path << std::endl;
		std::cout << "location_path: " << location_path << std::endl;
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
		// file not foundかそれ以外かをどう見分けるか
		status_code = 404;
		std::cout << "ko" << std::endl;
	}
	std::ostringstream oss;
	oss << ifs.rdbuf();
	status_code = 200;
	const std::string strs = oss.str();
	set_body(strs);
	std::cout << body << std::endl;
}

void HttpRes::write_file() {
	std::cout << "Write" << std::endl;
	std::string file_name = join_path();
	std::cout << "file_name: " << file_name << std::endl;
	std::ofstream ofs(file_name);
	if (!ofs.is_open()) {
		std::cout << "Error(ofstream)" << std::endl;
	}
	ofs << httpreq.getContentBody();
}

void HttpRes::delete_file() {
	std::cout << "Delete" << std::endl;
	std::string file_name = join_path();
	std::cout << "file_name: " << file_name << std::endl;
	// cのremoveなら使っても大丈夫??
	// c++だと新しくてダメっぽい
	if (remove(file_name.c_str())) {

	}
}
/*
std::string HttpRes::get_path(std::string str) {
}*/

std::string HttpRes::getStatusString() {
	switch (status_code) {
		case 200:
			return "OK\n";
		case 404:
			return "Not Found\n";
	}
	return "Error(statusString)\n";
}

void HttpRes::createControlData() {
	//body += httpreq.getVersion();
	header += "HTTP1.1 ";
	std::stringstream ss;
	std::string code;
	std::cout << "status_code: " << status_code << std::endl;
	ss << status_code;
	std::cout << "code: " << code << std::endl;
	header += ss.str();
	header += " ";
	header += getStatusString();
}

//void HttpRes::createDate()
void HttpRes::createDate(time_t now, std::string fieldName)
{
    char buf[1000];
    //time_t now = time(0);
    struct tm tm = *gmtime(&now);
    std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S ", &tm);
    //body += "Date: ";
	header += fieldName + ": ";
    std::string date(buf);
    header += date + "GMT\n";
}

void HttpRes::createContentLength() {
	std::stringstream ss;
	std::string code;
	ss << body.length();
	std::cout << "code: " << code << std::endl;
	header += "Content-Length: ";
	header += ss.str();
}

void HttpRes::set_content_type() {
	std::string ext;
	std::string::size_type dot_pos = uri.find('.');

	// .のみのケースに対応できるか？
	if (dot_pos != std::string::npos) {
		ext = uri.substr(dot_pos + 1);
		if (ext.length() == 0) {
			std::cerr << "Error dot" << std::endl;
		}
	}
	for (size_t i = 0; i < ext.length(); i++) {
		if (ext[i] >= 'A' && ext[i] <= 'Z') {
			type += std::tolower(ext[i]);
		}
	}
	// content-typeが受けられるか
	if (type) {
	}

	// マッチしなかったらデフォルトの値をセットする
	content_type_len = target.default_type;
}

void HttpRes::static_handler() {
	if (method != "GET" && method != "HEAD") {
		// なんてエラー返そう？
		return ;
	}

	if (uri[uri.length() - 1] == '/') {
		// なんて返す？
		return ;
	}

	//rc = ngx_http_discard_body(r);

	// map_uri_to_path();

	int fd;
	std::string file_name = join_path();
	fd = open(file_name);
	// open エラー
	if (fd == -1) {
		if ( errno == ENOENT || errno == ENOTDIR || errno == ENAMETOOLONG) {
		} else if (errno == EACCES) {
		} else {
		}
		return ;
	}

	struct stat sb;
	// stat がエラーだったとき
	if (stat(file_name.c_str(), &sb) == -1) {
		std::cout << "Error(stat)" << std::endl;
	}
	// ディレクトリだった時
	if (S_ISDIR(sb.st_mode)) {
		Location config = target.uri2location(uri);
		if (location.headers["alias"].count() == 0 && location.headers["root"] == 0) {
		}
		location = uri;
	}
	// 通常ファイル
	if (!S_ISREG(sb.st_mode)) {
		// なんのエラー?
		return ;
	}
	status_code = 200;
	content_length_n = sb.st_size;
	last_modified_time = sb.st_mtime;
	set_content_type();
}

void HttpRes::createResponseHeader(struct stat sb) {
	createControlData();
    //createDate();
    createDate(time(0), "Date");
	createDate(sb.st_mtime, "Last-Modified");
	createContentLength();
//    createServerName();
	std::cout << header<< std::endl;
}

void HttpRes::createResponse() {
	std::cout << httpreq << std::endl;
	// 一つもマッチしない場合は？
	target = longestMatchLocation(httpreq.getUri(), vServer.get_locations());
	//std::string request_path = get_path(httpreq.getUri());
	//target = longestMatchLocation(request_path, vServer.get_locations());
	std::cout << "target: " << target << std::endl;
	std::string method = httpreq.getMethod();
	struct stat sb;
	std::string file_name = join_path();
	if (stat(file_name.c_str(), &sb) == -1) {
		std::cout << "Error(stat)" << std::endl;
	}
	std::cout << "time: " << ctime(&sb.st_mtime) << std::endl;
	if (S_ISREG(sb.st_mode)) {
		if (isAllowMethod(method)) {
			if (method == "GET") {
				read_file();
			} else if (method == "POST") {
				write_file();
			} else if (method == "DELETE") {
				delete_file();
			}
			createResponseHeader(sb);
		}
	} else if (S_ISDIR(sb.st_mode)) {
	}
}
