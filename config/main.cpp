#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <string>

int main() {
	//std::ifstream ifs("simple_location.conf");
	std::ifstream ifs(".test/simple.conf");
	if (!ifs.is_open()) {
		std::cout << "Error" << std::endl;
		return 1;
	}

	std::ostringstream buf;
	buf << ifs.rdbuf();
	std::string contents = buf.str();
	Config config(contents);
	config.parse_config();
}
