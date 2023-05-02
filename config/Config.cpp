#include "Config.hpp"
#include <iostream>

Config::Config(std::string config_txt) {
	this->config_txt = config_txt;
}

Config::~Config() {
}

void Config::trim() {
	while (config_txt[idx] == ' ' ||
		config_txt[idx] == '\t') {
		idx++;
	}
}

void Config::expect_nl() {
	trim();
	std::cout << "expect_nl: " << config_txt[idx] << std::endl;
	if (config_txt[idx] == '\015') {
		if (config_txt[++idx] == '\012') {
			idx++;
			return;
		}
		//throw ;
		std::cout << "Error3" << std::endl;
	} else if (config_txt[idx] == '\012') {
		idx++;
	} else {
		//throw ;
		std::cout << "Error4" << std::endl;
		exit(1);
	}
}

void Config::expect(char c) {
	trim();
	if (config_txt[idx] != c) {
		//throw ;
		std::cout << "Error5" << std::endl;
		exit(1);
	}
	idx++;
}

Location Config::get_location() {
	Location location;
	std::string uri;
	std::string directive;

	uri = get_token(' ');
	location.set_uri(uri);
	trim();
	expect('{');
	expect_nl();
	while (idx != config_txt.length()) {
		trim();
		if (config_txt[idx] == '}') {
			idx++;
			break;
		}
		directive = get_token(' ');
		std::cout << "directive(location): " << directive << std::endl;
		//std::cout << "directive.length(location): " << directive.length() << std::endl;
		if (directive == "root") {
			location.set_root(get_token(';'));
		} else if (directive == "method") {
			location.set_method(get_token(';'));
		} else if (directive == "autoindex") {
			location.set_is_autoindex(get_token(';') == "Yes");
		} else if (directive == "upload_path") {
			location.set_upload_path(get_token(';'));
			/*
		} else if (directive == "max_body_size") {
			location.set_max_body_size(get_token(';'));
			*/
		} else if (directive == "index") {
			location.set_index(get_token(';'));
		} else if (directive == "cgi_path") {
			location.set_cgi_path(get_token(';'));
		} else {
			std::cout << "Error5" << std::endl;
			exit(1);
		}
		expect_nl();
	}
	std::cout << location << std::endl;
	return location;
}

std::string Config::get_token(char next_char) {
	std::string token = "";
	trim();
	while(idx < config_txt.length()) {
		if (config_txt[idx] == next_char) {
			break;
		}
		token += config_txt[idx];
		idx++;
	}
	// next_char文も読み飛ばす
	if (idx < config_txt.length())
		idx++;
	return token;
}


void Config::parse_config() {
	std::string directive;
	//bool is_block = false;
	directive = get_token(' ');
	if (directive != "server") {
		// Error handling
		std::cout << "Error1" << std::endl;
		exit(1);
	}
	trim();
	expect('{');
	expect_nl();
	// Serverの数も必要に応じて動的に
	configs.push_back(*(new VirtualServer()));
	// 空行、コメントアウト対応
	while (idx != config_txt.length()) {
		directive = get_token(' ');
		std::cout << "directive: " << directive << std::endl;
		if (directive == "listen") {
			configs[0].set_listen(get_token(';'));
		} else if (directive == "server_name") {
			configs[0].set_server_name(get_token(';'));
		} else if (directive == "index") {
			configs[0].set_index(get_token(';'));
		} else if (directive == "location") {
			configs[0].set_location(get_location());
		} else if (directive == "index") {
			configs[0].set_index(get_token(';'));
		} else if (directive == "root") {
			configs[0].set_index(get_token(';'));
		} else {
			std::cout << "Error2" << std::endl;
			exit(1);
		}
		expect_nl();
		trim();
		if (config_txt[idx] == '}') {
			idx++;
			break;
		}
	}
	std::cout << "===== result =====" << std::endl;
	std::cout << configs[0].get_locations()[0] << std::endl;
}
