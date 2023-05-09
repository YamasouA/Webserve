#include "configParser.hpp"

int main() {
	std::string txt = readConfFile("tmp2.conf");
	configParser confParser(txt);
	confParser.parseConf();
}
