#ifndef CONFIGPASER_HPP
#define CONFIGPASER_HPP

#include "virtualServer.hpp"
#include "Location.hpp"

class configParser {
	public:
		explicit configParser(const std::string& strs);
		configParser(const configPaser& src);
		configParser& operator=(const configPaser& rhs);
		~configParser();
		void configParser::parseConf();
	private:
		std::string buf;
		std::vector<virtualServer> serve_confs;
		size_t idx;

		void configParser::parseServe(size_t i);
		Location parseLocation();
		std::string configParser::getToken(char delimiter);
		std::string get_token_to_eol();
};

static std::vector<std::string> methodsSplit(std::string strs, char delimi);
void skip();
const std::string readConfFile(const std::string& file_name);
#endif
