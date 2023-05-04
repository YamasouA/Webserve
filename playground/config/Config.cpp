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

void Config::skip_nl() {
	std::cout << "skip_nl: " << config_txt[idx] << std::endl;
	if (config_txt[idx] == '\015') {
		if (config_txt[idx+1] == '\012') {
			idx+=2;
			return;
		}
	} else if (config_txt[idx] == '\012') {
		idx++;
	}
}

bool Config::is_skip_target() {
	if (config_txt[idx] == ' ' || config_txt[idx] == '\t')
		return true;
	else if (config_txt[idx] == '\012' || (config_txt[idx] == '\015' && config_txt[idx+1] == '\012')) {
		return true;
	}
	return false;
}

void Config::skip() {
	while (is_skip_target()) {
		std::cout << "config: " << config_txt[idx] << std::endl;
		trim();
		skip_nl();
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
	//trim();
	std::cout << "expect: " << config_txt[idx] << std::endl;
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
		if (directive[0] == '#') {
			get_token_to_eol();
			skip();
			continue;
		}
		//std::cout << "directive.length(location): " << directive.length() << std::endl;
		if (directive == "root") {
			location.set_root(get_token(';'));
		} else if (directive == "method") {
			location.set_method(get_token(';'));
		} else if (directive == "autoindex") {
			location.set_is_autoindex(get_token(';') == "Yes");
		} else if (directive == "upload_path") {
			location.set_upload_path(get_token(';'));
		} else if (directive == "max_body_size") {
			// 文字列から数字への変換適当
			std::stringstream sstream(get_token(';'));
			size_t result;
			sstream >> result;
			//location.set_max_body_size(get_token(';'));
			location.set_max_body_size(result);
		} else if (directive == "index") {
			location.set_index(get_token(';'));
		} else if (directive == "return") {
			location.set_return(get_token(';'));
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

std::string Config::get_token_to_eol() {
	std::string line = "";

	while (idx < config_txt.length()) {
		if (config_txt[idx] == '\015') {
			if (config_txt[idx+1] == '\012') {
				idx+=2;
				return line;
			}
		} else if (config_txt[idx] == '\012') {
			idx++;
			return line;
		}
		line += config_txt[idx];
		idx++;
	}

	// Error handling
	if (idx == config_txt.length()) {
		std::cout << "Error" << std::endl;
		exit(1);
	}
	return line;
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
	size_t	i = 0;
	//bool is_block = false;
	while (idx != config_txt.length()) {
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
			if (directive[0] == '#') {
				get_token_to_eol();
				skip();
				continue;
			}
			if (directive == "listen") {
				configs[i].set_listen(get_token(';'));
			} else if (directive == "server_name") {
				configs[i].set_server_name(get_token(';'));
			} else if (directive == "index") {
				configs[i].set_index(get_token(';'));
			} else if (directive == "location") {
				configs[i].set_location(get_location());
			} else if (directive == "index") {
				configs[i].set_index(get_token(';'));
			} else if (directive == "root") {
				configs[i].set_index(get_token(';'));
			} else {
				std::cout << "Error2" << std::endl;
				exit(1);
			}
			//expect_nl();
			//trim();

			skip();
			if (config_txt[idx] == '}') {
				std::cout << "kakko" << std::endl;
				idx++;
				break;
			}
		}
		std::cout << "===== result =====" << std::endl;
		std::cout << configs[0].get_locations()[0] << std::endl;
		i++;
		//expect_nl();
		//trim();
		skip();
		//expect('}');
		//skip();
		//expect_nl();
	}
}
