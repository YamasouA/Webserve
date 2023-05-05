#include "configPaser.hpp"

configPaser::configPaser(const std::string& strs)
:buf(strs),
	idx(0)
{}

configPaser::configPaser(const configPaser& src)
{}

configPaser& configPaser::operator=(const configPaser rhs)
{}

configPaser::~configPaser()
{}

const std::string readConfFile(const std::string& file_name)
{
	std::ifstream ifs(file_name);
	if (!ifs) {
		std::cout << "ko" << std::endl;
	}
	std::ostringstream oss;
	oss << ifs.rdbuf();
	const std::string strs;
	strs = oss.str();
	return strs;
}

std::string Config::get_token_to_eol() {
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

std::string Config::get_token(char delimiter)
{
	std::string token = "";
	while(idx < buf.length()) {
		if (buf[idx] == delimiter) {
			break;
		}
		token += buf[idx];
		idx++;
	}
	if (idx == buf.length())
		std::cout << "ko" << std::endl;
		//throw ;
	}
	idx++;
	return token;
}

config::parseServe(size_t i) {
	std::string directive;
	//std::string value;
	//size_t i = 0;
	while (idx != buf.size()) {
		directive = getToken(' ');
		if (directive[0] == '#') {
			get_token_to_eol();
			skip();
			continue;
		}
		skip();
		value = getToken(';');
		if (directive == "listen") {
			serve_confs[i].set_listen(value);
		} else if (directive == "") {
		}
		if (buf[idx] == '}') {
			idx++;
			break;
		}
	}
}

configPaser::paseConf()
{
	std::string directive;
	//std::string value;
	size_t i = 0;

	while (idx != buf.size()) {
		std::string directive = getToken(' ');
		if (directive != "server") {
			std::cerr << "Error1" << std::endl;
			std::exit(1);
		}
		skip(); // 空白などの読み飛ばし
		expect('{'); // 必須文字
		serve_confs.push_back(new virtualServer());
		parseServe(i);
		i++;
	}
}


