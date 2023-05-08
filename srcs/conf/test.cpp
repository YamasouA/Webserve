#include "configParser.hpp"

int main() {
	std::string txt = readConfFile("tmp.conf");
	configParser confParser(txt);
	confParser.parseConf();
}
