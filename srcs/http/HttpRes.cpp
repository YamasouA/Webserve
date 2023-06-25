#include "HttpRes.hpp"
#include "../Client.hpp"

const std::string HttpRes::kServerName = "WebServe";
const std::string HttpRes::default_type = "text/html";
//static const std::map<std::string, std::string> types = {{"html", "text/html"},{"json", "application/json"}};

std::string getContentType(std::string type) {
	if (type == "html")
		return "text/html";
	else if (type == "json")
		return "application/json";

	return "";
}

HttpRes::HttpRes() {

}

HttpRes::HttpRes(const Client& source, Kqueue kq)
:is_posted(0)
{
	//this->httpreq = source.get_parsedReq();
	this->httpreq = source.get_httpReq();
	this->vServer = source.get_vServer();
    this->connection = kq;
	this->fd = source.get_fd();
}

HttpRes::HttpRes(const HttpRes& src) {
    this->buf = src.buf;
    this->header_size = src.header_size;
    this->out_buf = src.out_buf;
    this->body_size = src.body_size;
}

HttpRes::~HttpRes() {
}


Location HttpRes::get_uri2location(std::string uri) const
{
	std::cout << "===== get_uri2location() =====" << std::endl;
//    std::vector<Location> locs = vServer.get_locations();
//    std::map<std::string, Location> uri2loc;
//    for (std::vector<Location>::iterator it = locs.begin(); it != locs.end(); ++it) {
//        uri2loc[it->get_uri()] = *it;
//    }
	std::map<std::string, Location> uri2location = vServer.get_uri2location();
	std::map<std::string, Location>::const_iterator loc = uri2location.find(uri);
	std::cout << "request uri: " << uri << std::endl;
	std::cout << "size: " << uri2location.size() << std::endl;
	for (std::map<std::string, Location>::iterator it = uri2location.begin();
		it != uri2location.end(); it++) {
		std::cout << "location uri: " << it->second.get_uri() << std::endl;
	}
	if (loc != uri2location.end()) {
        std::cout << "match all" << std::endl;
		return loc->second;
	}
	/*
	if (uri2location.count(uri) == 1)
		return uri2location[uri];
	*/
	std::string path = uri;
	while (1) {
		std::string::size_type i = path.rfind('/');
//		std::cout << "i: " << i << std::endl;
//		std::cout << "a" << std::endl;
		if (i == std::string::npos) {
			std::cout << "a" << std::endl;
            std::cout << "okend" << std::endl;
			break;
		}
//		std::cout << "b" << std::endl;
		std::cout << "path(bef): " << path << std::endl;
		if (i == 0) {
			path = path.substr(0, 1);
        } else {
		    path = path.substr(0, i);
        }
		//std::cout << "path: " << path << std::endl;
//        }
        std::cout << "substr path: " << path << std::endl;
		loc = uri2location.find(path);
//		std::map<std::string, Location>::const_iterator loc = uri2location.find(path);
		if (loc != uri2location.end()) {
            std::cout << "ok1" << std::endl;
			return loc->second;
		}

//		if (uri2location.count(path) == 1)
//			return uri2location[path];

	}

	std::cout << "break" << std::endl;
	// 何もマッチしなかった場合の挙動イズなに？
	return loc->second;
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
    std::cout << "===== join_path =====" << std::endl;
	std::string path_root = target.get_root();
    std::cout << "root: " << path_root << std::endl;
	//std::string file_path  = target.get_uri();
	std::string config_path  = target.get_uri();
	std::string file_path = httpreq.getUri().substr(config_path.length());
	std::cout << "config_path: " << config_path<< std::endl;
	std::cout << "Path: " << file_path << std::endl;
    //std::cout << "uri: " << file_path << std::endl;
	if (file_path.length() && file_path[file_path.length() - 1] == '/' && target.get_is_autoindex()) {
		file_path = "/index.html"; //  "/index" is better?
	}
	std::string alias;
	if ((alias = target.get_alias()) != "") {
		config_path = alias;
	}
    //std::cout << "not auto index" << std::endl;
    std::cout << "file_path(in join_path): " << file_path << std::endl;
	if (path_root[path_root.length() - 1] == '/') {
		//file_path = file_path.substr(1);
		config_path = config_path.substr(1);
	}
	if (config_path == "" || config_path[config_path.length() - 1] == '/') {
		//file_path = file_path.substr(1);
		file_path = file_path.substr(1);
	}
	std::cout << "path: " << path_root + config_path + file_path << std::endl;
    std::cout << "===== End join_path =====" << std::endl;
	return path_root + config_path + file_path;
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
	/*
	if (types.count(type) != 0) {
		content_type = types[type];
	}*/
	content_type = getContentType(type);

	// マッチしなかったらデフォルトの値をセットする
	content_type = default_type;
}

void HttpRes::ev_queue_insert() {
	connection.set_event(fd, EVFILT_WRITE);
    std::cout << "send event!!!!!!!!!!!!!!!!!!!" << std::endl;
}

void HttpRes::post_event() {
    if (!is_posted) {
        is_posted = 1;
        ev_queue_insert();
    }
    std::cout << "posted event" << std::endl;
}

std::map<int, std::string> create_status_msg(){
    std::map<int, std::string> m;
    m[200] = "OK";
    m[201] = "Created";
    m[202] = "Accepted";
    m[203] = "";
    m[204] = "No Content";
    m[205] = "";
    m[206] = "Partial Content";
    m[301] = "Moved Permanently";
    m[302] = "Moved Temporarily";
    m[303] = "See Other";
    m[304] = "Not Modified";
    m[307] = "Temporary Redirect";
    m[308] = "Permanent Redirect";

    m[400] = "Bad Request";
    m[401] = "Unauthorized";
    m[402] = "Payment Required";
    m[403] = "Forbidden";
    m[404] = "Not Found";
    m[405] = "Not Allowed";
    m[406] = "Not Acceptable";
    m[408] = "Request Time-out";
    m[409] = "Conflict";
    m[410] = "Gone";
    m[411] = "Length Required";
    m[412] = "Precondition Failed";
    m[413] = "Request Entity Too Large" ;
    m[414] = "Request-URI Too Large";
    m[415] = "Unsupported Media Type";
    m[416] = "Requested Range Not Satisfiable";
    m[421] = "Misdirected Request";
    m[429] = "Too Many Requests";

    m[500] = "Internal Server Error";
    m[501] = "Not Implemented";
    m[502] = "Bad Gateway";
    m[503] = "Service Temporarily Unavailable";
    m[504] = "Gateway Time-out";
    m[505] = "HTTP Version Not Supported";
    m[507] = "Insufficient Storage";
	return m;
}

int HttpRes::dav_depth() {
	// アクセスできるディレクトリの深さを返す
	int depth = target.get_depth();
	return depth;
}

typedef struct {
    DIR             *dir;
    struct dirent   *d_ent;
    struct stat     d_info;
    bool            valid_info:1;
} dir_t;


std::string HttpRes::join_dir_path(const std::string& dir_path, const std::string& elem_name) {
	return dir_path + '/' + elem_name;
}

void HttpRes::diving_through_dir(const std::string& path) {
    //class is better?
    dir_t dir_info;
    dir_info.dir = opendir(path.c_str());
    if (dir_info.dir == NULL) {
        std::cerr << "opendir Error" << std::endl;
    }
    dir_info.valid_info = 0;
    for (;;) {
        errno = 0;
        dir_info.d_ent = readdir(dir_info.dir);
        if (!dir_info.d_ent) {
            if (errno != 0) {
              std::cerr << "readdir Error" << std::endl;
          } else {
            // read directory end
            }
            if (closedir(dir_info.dir)) {
                std::cerr << "closedir error" << std::endl;
            }
            return;
              // done handle
        }
        std::string file_name(dir_info.d_ent->d_name);
        if (file_name.length() == 1 && file_name[0] == '.') {
            continue;
        }
        if (file_name.length() == 2 && file_name[0] == '.' && file_name[1] == '.') {
            continue;
        }
        std::string abs_path = join_dir_path(path, file_name);
        std::cout << "abs_path: " << abs_path << std::endl;

        if (!dir_info.valid_info) {
//            dir_info.type = 0;
            if (stat(abs_path.c_str(), &(dir_info.d_info)) == -1) {
                continue;
            }
        }
        if ((S_ISREG(dir_info.d_info.st_mode))) {
            if (remove(abs_path.c_str()) < 0) {
                std::cerr << "remove error" << std::endl;
                status_code = INTERNAL_SERVER_ERROR;
                if (closedir(dir_info.dir)) {
                    std::cerr << "closedir error" << std::endl;
                }
                return;
            }
        } else if ((S_ISDIR(dir_info.d_info.st_mode))) {
            diving_through_dir(abs_path);
            if (status_code == INTERNAL_SERVER_ERROR) {
                if (closedir(dir_info.dir)) {
                    std::cerr << "closedir error" << std::endl;
                }
                return;
            }
            if (rmdir(abs_path.c_str()) < 0) {
                std::cerr << "rmdir error" << std::endl;
                status_code = INTERNAL_SERVER_ERROR;
                if (closedir(dir_info.dir)) {
                    std::cerr << "closedir error" << std::endl;
                }
                return;
            }
        } else {
            if (remove(abs_path.c_str()) < 0) {
                std::cerr << "remove error" << std::endl;
                status_code = INTERNAL_SERVER_ERROR;
                if (closedir(dir_info.dir)) {
                    std::cerr << "closedir error" << std::endl;
                }
                return;
            }
      }
    }
}

void HttpRes::dav_delete_path(bool is_dir) {
    std::cout << "dav_delete_path" << std::endl;
    if (is_dir) {
        std::string dir_path = join_path();
        std::cout << "dir_path: " << dir_path << std::endl;
        diving_through_dir(dir_path);
        if (status_code == INTERNAL_SERVER_ERROR) {
            return;
        }
        if (rmdir(dir_path.c_str()) < 0) {
			std::cout << "delete error" << std::endl;
			status_code = INTERNAL_SERVER_ERROR;
            return;
        }

		// 本当ならディレクトリ配下を確認して問題なければディレクトリを消すべき
		// めんどいからディレクトリの削除を行わせない？
		status_code = BAD_REQUEST;
	} else {
		std::string file_name = join_path();
		//file_name = "hogehoge.txt";
		std::cout << "delete!!" << std::endl;
		if (remove(file_name.c_str()) < 0) {
			std::cout << "delete error" << std::endl;
			status_code = INTERNAL_SERVER_ERROR;
			return;
		}
		status_code = OK;
	}
}

void HttpRes::dav_delete_handler() {
	std::cout << "\n\n=====dav delete handler=====" << std::endl;
	int content_length = httpreq.getContentLength();
	if (content_length > 0) {
		status_code = UNSUPPORTED_MEDIA_TYPE;
	}

	struct stat sb;
	bool is_dir;
	int depth;
	std::string method = httpreq.getMethod();
	if (method != "DELETE") {
		return;
	}

	std::vector<std::string> allow_methods = target.get_methods();
	std::cout << "allow_methods: " << std::endl;
	if (find(allow_methods.begin(), allow_methods.end(), method) == allow_methods.end()) {
		std::cout << "not allow (conf)" << std::endl;
		status_code = BAD_REQUEST;
		return ;
	}

	std::string file_name = join_path();
	file_name = "abc";
    if (stat(file_name.c_str(), &sb) == -1) {
		std::cout << "Error(stat)" << std::endl;
		status_code = INTERNAL_SERVER_ERROR;
		return;
	}
	if (S_ISDIR(sb.st_mode)) {
		std::string uri = httpreq.getUri();
        std::cout << "dir uri: " << uri << std::endl;
//		if (uri[uri.length() - 1] != '/') {
//			status_code = BAD_REQUEST;
//			return;
//		}
        std::cout << "ok1" << std::endl;
		depth = dav_depth();
		if (depth != -1) {
			status_code = BAD_REQUEST;
			return;
		}
		is_dir = true;
        std::cout << "ok" << std::endl;
	} else {
		std::cout << "delete files" << std::endl;
		depth = dav_depth();
		std::cout << "depth: " << depth << std::endl;
		if (depth != 0 && depth != -1) {
			status_code = BAD_REQUEST;
			return;
		}
		is_dir = false;
	}
	dav_delete_path(is_dir);
}

void HttpRes::header_filter() {
	// ステータスがOKでないならlast_modifiedは消す
	std::map<int, std::string> status_msg = create_status_msg();
	if (last_modified_time != -1) {
		if (status_code != HTTP_OK) {
			last_modified_time = -1;
		}
	}

	// status_lineの作成
	if (status_line == "") {
        std::stringstream ss;
        ss << status_code;
        std::string status_code_str = ss.str();
        ss >> status_code_str;
		if (status_code >= INTERNAL_SERVER_ERROR) {
            // 5XX
            status_line = "HTTP/1.1 " + status_code_str + ' ' + status_msg[status_code];
		} else if (status_code >= BAD_REQUEST) {
            // 4XX
            status_line = "HTTP/1.1 " + status_code_str + ' ' +  status_msg[status_code];
		}
        else if (status_code >= MOVED_PERMANENTLY) {
            // 3XX
            if (status_code == NOT_MODIFIED) {
                header_only = 1;
            }
            status_line = "HTTP/1.1 " + status_code_str + ' ' +  status_msg[status_code];
		} else if (status_code >= HTTP_OK) {
            // 2XX
            if (status_code == NOT_MODIFIED) {
                header_only = 1;
            }
            if (status_code == NO_CONTENT) {
                header_only = 1;
                content_type = "";
                last_modified = NULL;
            }
            status_line = "HTTP/1.1 " + status_code_str + ' ' +  status_msg[status_code];
        } else {
            //
			status_line = "";
		}
	}

	// めっちゃlenを操作してる箇所はいらなさそうだから飛ばす

	buf += status_line;
	buf += "\r\n";
	// ServerNameも設定できるぽいけど挙動よくわからん
	buf += "Server: " + kServerName;

	buf += "\r\n";
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
//		buf += "\r\n";
	}

	// Locationの処理いろいろやってそう

	// chunkedは無視

	// keep-alive系は問答無用でcloseする？
    std::map<std::string, std::string> header_fields = httpreq.getHeaderFields();
    if (header_fields["connection"] == "keep-alive") {
        buf += "Connection: keep-alive";
    } else {
	    buf += "Connection: close";
    }
	//buf += "Connection: close";
	buf += "\r\n";

	// 残りのヘッダー  もしかしたら必要ないかも？ 現状Connection filedなどがダブってしまっているetc...
	std::map<std::string, std::string> headers = httpreq.getHeaderFields();
	std::map<std::string, std::string>::iterator it= headers.begin();
	for (; it != headers.end(); it++) {
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

void HttpRes::sendHeader() {
    // check alredy sent
    // handle err_status?
    return header_filter();
}

int HttpRes::static_handler() {
	std::cout << "===== static_handler =====" << std::endl;
	std::string uri = httpreq.getUri();
	target = get_uri2location(uri);
    std::cout << "macth loc: " << target << std::endl;
	std::string method = httpreq.getMethod();
	if (method != "GET" && method != "HEAD" && method != "POST") {
        std::cerr << "not allow method(405)" << std::endl;
		// なんてエラー返そう？
		return DECLINED;
	}
	std::vector<std::string> allow_methods = target.get_methods();
	std::cout << "allow_methods: " << std::endl;
	if (find(allow_methods.begin(), allow_methods.end(), method) == allow_methods.end()) {
		std::cout << "not allow (conf)" << std::endl;
		status_code = BAD_REQUEST;
		return DECLINED;
	}

	if (uri[uri.length() - 1] == '/') {
        //move next handler
		// なんて返す？ (declined)
		return DECLINED;
	}

	//rc = ngx_http_discard_body(r);

	//map_uri_to_path();

	int fd = -1;
	std::string file_name = join_path();
	//file_name = "index.html";
    std::cout << "file_name: " << file_name << std::endl;
	std::cout << "method: " << method << std::endl;
    struct stat sb;
    status_code = 200;
    if (method == "GET") {
        fd = open(file_name.c_str(), O_RDONLY);
        if (fd == -1) {
            std::cerr << "open error" << std::endl;
            if (errno == ENOENT || errno == ENOTDIR || errno == ENAMETOOLONG) {
                return NOT_FOUND;
            } else if (EACCES){
                return FORBIDDEN;
            }
            // 次のハンドラーに処理を託す
            return INTERNAL_SERVER_ERROR;
        }
        // stat がエラーだったとき
        if (stat(file_name.c_str(), &sb) == -1) {
            std::cout << "GET Error(stat)" << std::endl;
        }
        // ディレクトリだった時
        if (S_ISDIR(sb.st_mode)) {
            return DECLINED;
        }
    //    if (!S_ISREG(sb.st_mode) && method == "POST") {
    //        //status 405
    //        std::cerr << "NOT ALLOW METHOD" << std::endl;
    //        return NOT_ALLOWED;
    //    }
        // 通常ファイルではない
        if (!S_ISREG(sb.st_mode)) {
            // なんのエラー?
            std::cerr << "NOT FOUND(404)" << std::endl;
            //status 404
            return NOT_FOUND;
        }
	    content_length_n = sb.st_size;
	    last_modified_time = sb.st_mtime;
    } else if (method == "POST") {
        if (stat(file_name.c_str(), &sb) == -1) {
			// ファイルが存在しない
			if (errno != ENOENT) {
                std::cerr << "stat error" << std::endl;
                return INTERNAL_SERVER_ERROR;
				// throw error
			}
			status_code = 201;
        }
        // ディレクトリだった時
        if (S_ISDIR(sb.st_mode)) {
            return DECLINED;
        }
    //    if (!S_ISREG(sb.st_mode) && method == "POST") {
    //        //status 405
    //        std::cerr << "NOT ALLOW METHOD" << std::endl;
    //        return NOT_ALLOWED;
    //    }
        // 通常ファイルではない
        content_length_n = sb.st_size;
	    last_modified_time = sb.st_mtime;
        if (!S_ISREG(sb.st_mode) && status_code != 201) {
			std::cerr << "stat error" << std::endl;
        	return INTERNAL_SERVER_ERROR;
			/*
            fd = open(file_name.cstr(), O_CREATE | O_WRONLY); //open necessary???
            if (fd == -1) {
                std::cerr << "open error" << std::endl;
                return INTERNAL_SERVER_ERROR;
            }
            status_code = 201;
			*/

        } else {
			// 通常ファイル
			//file_name = "sinki.html";
			fd = open(file_name.c_str(), O_CREAT | O_WRONLY | O_APPEND, 00644);
			if (fd == -1) {
                std::cerr << "POST open error" << std::endl;
                return INTERNAL_SERVER_ERROR;
			}
			std::string body = httpreq.getContentBody().c_str();
			std::cout << "POST body: \n" << body << std::endl;
			write(fd, body.c_str(), body.size());
		}
    }
    close(fd);
    //discoard request body here ?
	set_content_type();
    //set_etag(); //necessary?
    std::cout << "just before sendHeader" << std::endl;
    sendHeader();

//    init_res_body();

    std::ifstream ifs(file_name.c_str());
    if (!ifs) {
        std::cerr << "ifstream ko" << std::endl;
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    out_buf = oss.str();
	//buf += out_buf;
    std::cout << "body: " << out_buf << std::endl;
    body_size = content_length_n;
	std::cout << "===== End static_handler =====" << std::endl;
    return OK;
//    output_filter();
}

void HttpRes::runHandlers() {
    static_handler();
	dav_delete_handler();
}
