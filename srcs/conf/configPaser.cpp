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

configPaser::paseConf()
{
	while (idx != buf.size()) {
		std::string directive = getToken(' ');
		if (directive != "server") {
			std::cerr << "Error1" << std::endl;
			std::exit(1);
		}
	}
}


