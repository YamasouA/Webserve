#include "configParser.hpp"

int main() {
	std::string txt = readConfFile("conf.conf");
	configParser confParser(txt);
	confParser.parseConf();
}
