#include "HttpRes.hpp"
#include "../Client.hpp"

HttpRes::HttpRes(const Client& source, Kqueue kq) {
	//this->httpreq = source.get_parsedReq();
	this->httpreq = source.get_httpReq();
	this->vServer = source.get_vServer();
    this->connection = kq;
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
	std::string type;
	std::string uri = httpreq.getUri();
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
		} else {
			type += ext[i];
		}
	}

	// content-typeが受けられるか
	if (types.count(type) != 0) {
		content_type = types[type];
	}

	// マッチしなかったらデフォルトの値をセットする
	content_type = default_type;
}

void ev_queue_insert() {

}

void HttpRes::post_event() {
    if (!is_posted) {
        is_posted = 1;
        ev_queue_insert();
    }
}

void HttpRes::header_filter() {
	// ステータスがOKでないならlast_modifiedは消す
	if (last_modified_time != -1) {
		if (status_code != HTTP_OK) {
			last_modified_time = -1;
		}
	}

	// status_lineの作成
	if (status_line == NULL) {

		if (status_code >= INTERNAL_SERVER_ERROR) {
		} else if (status_code >= BAD_REQUEST) {
            const std::vector<std::string> status_success[30] = {NULL, "400 Bad Request",};
		}
        else if (status_code >= MOVED_PERMANENTLY) {
            const std::vector<std::string> status_success[9] = {NULL, "301 Moved Permanently", "302 Moved Temporarily", "303 See Other", "304 Not Modied", NULL, NULL, "307 Temporary Redirect", "308 Permanent Redirect"};
            if (status_code == NOT_MODIFIED) {
                header_only = 1;
            }
            status_line = "HTTP/1.1 " + status_redirect[status_code - MOVED_PERMANENTLY];
		} else if (status_code >= OK) {
            const std::vector<std::string> status_success[7] = {"200 OK", "201 Created", "202 Accepted", NULL, "204 No Content", NULL, "206 Partial Content"};
            if (status_code == NO_CONTENT) {
                header_only = 1;
                content_type = NULL;
                last_modified = NULL;
                content_length = NULL;
            }
            status_line = "HTTP/1.1 " + status_success[status_code - OK];
        } else {
			status_line = NULL;
		}
	}

	// めっちゃlenを操作してる箇所はいらなさそうだから飛ばす
	//Location loc = httpreq.get_uri2location(uri);

	buf += status_line;
	buf += "\r\n";
	// ServerNameも設定できるぽいけど挙動よくわからん
	buf += "Server: " + kServerName;

	// Cacheとかは考慮しないのでdateの処理は飛ばす

	if (content_type != "") {
		buf += "Content-Type: " + content_type;

		if (charset != "") {
			buf += "; charset=" + charset;

			// content_type に charsetを加える
		}
		buf += "\r\n";
	}

	// content_length_n と content_lengthの関係がよくわからん
	if (last_modified_time != -1) {
		//buf += "Last-Modified: " + http_time();
		buf += "\r\n";
	}

	// Locationの処理いろいろやってそう

	// chunkedは無視

	// keep-alive系は問答無用でcloseする？
	buf += "Connection: close";
	buf += "\r\n";

	// 残りのヘッダー
	std::map<std::string, std::string> headers = httpreq.getHeaderFields();
	std::map<std::string, std::string>::iterator it= headers().begin();
	for (; it != it.end(); it++) {
		buf += it->first;
		buf += ": ";
		buf += it->second;
		buf += "\r\n";
	}
	buf += "\r\n";
	header_size = buf.size();

	//
    post_event();
	//write_filter();
}

void HttpRes::static_handler() {
	if (method != "GET" && method != "HEAD" && method != "POST") {
        std::cerr << "not allow method(405)" << std::endl;
		// なんてエラー返そう？
		return ;
	}

	if (uri[uri.length() - 1] == '/') {
        //move next handler
		// なんて返す？ (declined)
		return ;
	}

	//rc = ngx_http_discard_body(r);

	map_uri_to_path();

    //disable symlink ?

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
    if (!S_ISREG(sb.st_mode) && method == "POST") {
        //status 405
        std::cerr << "NOT ALLOW METHOD" << std::endl;
        return ;
    }
	// 通常ファイルではない
	if (!S_ISREG(sb.st_mode)) {
		// なんのエラー?
        std::cerr << "NOT FOUND(404)" << std::endl;
        //status 404
		return ;
	}
    //discoard request body here ?
	status_code = 200;
	content_length_n = sb.st_size;
	last_modified_time = sb.st_mtime;
	set_content_type();
    //set_etag(); //necessary?

    sendHeader();

    init_res_body();

    output_filter();
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
