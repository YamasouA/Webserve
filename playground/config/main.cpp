#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <strings.h>
void config_parse_test(std::string filename) {
	std::cout << "===== " << filename << " =====" << std::endl;
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		std::cout << "Error" << std::endl;
		return;
	}

	std::ostringstream buf;
	buf << ifs.rdbuf();
	std::string contents = buf.str();
	Config config(contents);
	config.parse_config();
}

int main(int argc, char *argv[]) {
	//std::ifstream ifs("simple_location.conf");
	if (argc == 1) {
		config_parse_test(".test/simple.conf");
	}
	for (int i = 1; i < argc; i++) {
		config_parse_test(".test/" + std::string(argv[i]));
	}
}
