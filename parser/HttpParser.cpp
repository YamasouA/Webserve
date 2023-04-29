#include "HttpParser.hpp"

HttpParser::HttpParser(std::string request): rmsg(request), rmsg_len(request.length()) {}

HttpParser::~HttpParser() {}

HttpParser::HttpParser(const HttpParser& source) {
	if (this == &source) {
		return;
	}
	this->headers = source.headers;
	this->version = source.version;
	this->method = source.method;
	this->rmsg = source.rmsg;
	this->rmsg_len = source.rmsg.length();
}

HttpParser& HttpParser::operator =(const HttpParser& source) {
	this->headers = source.headers;
	this->version = source.version;
	this->method = source.method;
	this->rmsg = source.rmsg;
	this->rmsg_len = source.rmsg.length();
	return *this;
}

std::string HttpParser::getVersion() {
	return version;
}

std::string HttpParser::getMethod() {
	return method;
}

std::string HttpParser::trim(std::string src) {
	size_t start = src.find_first_not_of(" \t");
	size_t end = src.find_last_not_of(" \t");
	std::string dst = src;
	/*
	if (start != std::string::npos && end != std::string::npos) {
		std::string dst = src.substr(start, end - start + 1);
		return dst;
	}*/
	if (start != std::string::npos) {
		dst = dst.substr(start);
	}
	if (end != std::string::npos) {
		dst = dst.substr(0, end - start + 1);
	}
	//std::cout << "dst: " << dst << std::endl;
	return dst;
}

std::string HttpParser::get_token(char next_char) {
	size_t i = 0;
	std::string token;

	while (1) {
		if (i == rmsg.length())
			break;
		if (rmsg[i] == next_char) {
			break;
		}
		i++;
	}
	token = rmsg.substr(0, i);
	rmsg_len -= token.length();
	token = trim(token);
	rmsg = rmsg.substr(i);
	rmsg = trim(rmsg);
	return token;
}

std::string HttpParser::get_token_to_eol() {
	size_t i = 0;
	size_t trim_n = 0;
	std::string token;

	while (1) {
		if (i == rmsg.length())
			break;
		if (rmsg[i] == '\015') {
			if (rmsg[i+1] == '\012') {
				trim_n = 2;
				break;
			}
			return "";
		} else if (rmsg[i] == '\012') {
			trim_n = 1;
			break;
		}
		i++;
	}
	token = rmsg.substr(0, i);
	rmsg_len -= token.length();
	token = trim(token);
	rmsg = rmsg.substr(i);
	rmsg = trim(rmsg);
	//std::cout << "trim_n: " << trim_n << std::endl;
	rmsg = rmsg.substr(trim_n);
	//std::cout << "get_token: " << rmsg << std::endl;
	return token;
}

void HttpParser::parse_headers() {
	std::string field;
	std::string value;

	while (1) {
		if (rmsg[0] == '\015') {
			//std::cout << "Hoge" << std::endl;
			if (rmsg[1] == '\012') {
				//std::cout << "Hoge2" << std::endl;
				//break;
				rmsg = rmsg.substr(2);
				//std::cout << "rmsg: " << rmsg << std::endl;
				break;
				if (rmsg.length() == 0)
					break;
			}
			else {
				//std::cout << "Hoge3" << std::endl;
			}
		} else if (rmsg[0] == '\012') {
			break;
		}
		//field = get_token(':');
		if ((field = get_token(':')) == "") {
			//std::cout << "Error3" << std::endl;
			//exit(1);
			break;
		} else {
			//rmsg = rmsg.substr(1);
			if (rmsg[0] == ':')
				rmsg = rmsg.substr(1);
			value = get_token_to_eol();
			headers[field] = value;
			//std::cout << "field: " << field << " value: " << value << std::endl;
		}
	}
}

void HttpParser::parse_request() {
	// Error handling
	if ((method = get_token(' ')) == "") {
		exit(1);
	}
	if ((resource = get_token(' ')) == "") {
		exit(1);
	}
	//if ((version = get_token(' ')) == "") {
	if ((version = get_token_to_eol()) == "") {
		exit(1);
	}
	std::cout << "method: " << method << std::endl;
	std::cout << "resource: " << resource << std::endl;
	std::cout << "version: " << version << std::endl;
	std::cout << "rmsg: " << rmsg << std::endl;
	/*
	if (rmsg[0] == '\015') {
		if (rmsg[1] != '\012') {
			std::cout << "Error1" << std::endl;
			return ;
		}
		rmsg = rmsg.substr(2);
	} else if (rmsg[0] == '\012') {
		std::cout << "fuck" << std::endl;
		rmsg = rmsg.substr(1);
	} else {
		//std::cout << "Error2" << std::endl;
		//return;
	}*/
	parse_headers();
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
		std::cout << "key: " << it->first << "a"  << " value: " << it->second << std::endl;
	}
	/*
	while(1) {
		check_eof();
		if (*rmsg == '\015') {
			rmsg++;
			EXPECT_CHAR('\012');
			break;
		} else if (*rmsg == '\012') {
			rmsg++;
			break;
		}
		if (

	}*/
}
