#include "httpReq.hpp"
#include <sstream>
#include <fstream>

static const std::string readConfFile(const std::string& file_name)
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

int main() {
	httpReq httpParser(readConfFile("test.http"));
	httpParser.parseRequest();

	//std::cout << httpParser << std::endl;
}
