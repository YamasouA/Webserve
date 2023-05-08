#include "configParser.hpp"

configParser::configParser(const std::string& strs)
:buf(strs),
	idx(0)
{}

configParser::configParser(const configParser& src)
{
	(void)src;
}

configParser& configParser::operator=(const configParser& rhs)
{
	if (this == &rhs) {
		return *this;
	}
	return *this;
}

configParser::~configParser()
{}

const std::string readConfFile(const std::string& file_name)
{
	std::ifstream ifs(file_name.c_str());
	if (!ifs) {
		std::cout << "ko" << std::endl;
	}
	std::ostringstream oss;
	oss << ifs.rdbuf();
	const std::string strs = oss.str();
	return strs;
}

std::string configParser::get_token_to_eol() {
	std::string line = "";
	while (idx < buf.length()) {
		if (buf[idx] == '\015') {
			if (buf[idx+1] == '\012') {
				idx += 2;
				return line;
			}
		} else if (buf[idx] == '\012') {
			idx++;
			return line;
		}
		line += buf[idx];
		idx++;
	}
	return line;
}

void configParser::skip()
{
	while (buf[idx] == ' ' || buf[idx] == '\t'
		|| buf[idx] == '\015' || buf[idx] == '\012') { //|| buf[idx] == ';') {
		++idx;
	}
}

void configParser::trim(std::string& str)
{
	size_t i = 0;
//	std::cout << "trim str: " << str << std::endl;
	while (str[i] == ' ' || str[i] == '\t') {
		++i;
		std::cout << i << std::endl;
	}
	str.erase(0, i);
//	std::cout << "trimed str: " << str << std::endl;
}

std::string configParser::getToken(char delimiter)
{
	std::string token = "";
	while (buf[idx] == delimiter) {
		++idx;
	}
	while(idx < buf.length()) {
		if (buf[idx] == delimiter) {
			break;
		}
		token += buf[idx];
		idx++;
	}
	if (idx == buf.length()) {
		std::cout << "ko getToken" << std::endl;
		//throw ;
	}
//	std::cout << "token end" << buf[idx] << std::endl;
	idx++;
//	std::cout << "token end2" << buf[idx] << std::endl;
	skip();
	trim(token);
	return token;
}

//stringstreamを使わない実装の方が高速なので後でそちらに変更もあり
static std::vector<std::string> methodsSplit(const std::string &strs, char delimi)
{
	std::vector<std::string> methods;
	std::stringstream ss(strs);
	std::string method;

	while (std::getline(ss, method, delimi)) {
		if (!method.empty()) {
			methods.push_back(method);
		}
	}
	return methods;
}

Location configParser::parseLocation() {
	Location location;
	skip();
	// その他prefixは考慮するとめんどくさそう
	std::string uri = getToken('{');
	// 末尾に空白が入るかも(入らない可能性もある)
	trim(uri);
	location.set_uri(uri);
	skip();
	while (idx != buf.size()) {
		if (buf[idx] == '}') {
			idx++;
			break;
		}
		std::string directive = getToken(' ');
		if (directive[0] == '#') {
			get_token_to_eol();
			skip();
			continue;
		}
		if (directive == "root") {
			location.set_root(getToken(';'));
		} else if (directive == "method") {
			const std::string methods = getToken(';');
			location.set_methods(methodsSplit(methods, ' '));
			// ' 'か', 'でsplitしてベクターに変換して返す
		} else if (directive == "autoindex") {
			location.set_is_autoindex(getToken(';')=="on");
			// autoindexはbooleanで持つ？
		} else if (directive == "upload_path") {
			location.set_upload_path(getToken(';'));
			// ワンチャンupload_pathは公式のものじゃないかも
		} else if (directive == "max_body_size") {
			std::stringstream sstream(getToken(';'));
			size_t result;
			sstream >> result;
			if (sstream.fail() && std::numeric_limits<size_t>::max() == result) {
				std::cerr << "overflow" << std::endl;
			}
			location.set_max_body_size(result);
		}
		idx++;
	}
	return location;
}

void configParser::parseServe(size_t i) {
	std::string directive;
	//std::string value;
	//size_t i = 0;
	while (idx != buf.size()) {
		directive = getToken(' ');
//		std::cout << "directive: " << directive << std::endl;
		if (directive[0] == '#') {
			std::cout << directive << std::endl;
			get_token_to_eol();
			skip();
			continue;
		}
		skip();
		//value = getToken(';');
		if (directive == "listen") {
			serve_confs[i].set_listen(getToken(';'));
		} else if (directive == "server_name") {
			serve_confs[i].set_server_name(getToken(';'));
		} else if (directive == "root") {
			serve_confs[i].set_root(getToken(';'));
		} else if (directive == "location") {
			serve_confs[i].set_location(parseLocation());
//		} else if (directive == "") {
//			continue;
		} else {
//			continue;
			std::cerr << "directive Error" << std::endl;
		}
		if (buf[idx] == '}') {
			idx++;
			break;
		}
	}
	std::cout << serve_confs[i] << std::endl;
}

void configParser::expect(char c)
{
	if (buf[idx] != c) {
		std::cerr << "expected expression" << std::endl;
		std::exit(1);
	}
	++idx;
}

void configParser::parseConf()
{
	std::string directive;
	//std::string value;
	size_t i = 0;

	while (idx < buf.size()) {
		std::string directive = getToken(' ');
		if (directive != "server") {
			std::cerr << "Error1" << std::endl;
			std::exit(1);
		}
		skip(); // 空白などの読み飛ばし
		expect('{'); // 必須文字
		virtualServer virtual_server;
		serve_confs.push_back(virtual_server);
		parseServe(i);
		i++;
	}
}


