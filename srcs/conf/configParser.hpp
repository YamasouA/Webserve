#ifndef CONFIGPASER_HPP
#define CONFIGPASER_HPP

#include <fstream>
#include <sstream>
#include "virtualServer.hpp"
#include "Location.hpp"

class configParser {
	public:
		explicit configParser(const std::string& strs);
		configParser(const configParser& src);
		configParser& operator=(const configParser& rhs);
		~configParser();

		void parseConf();
		void skip();
		void trim(std::string& str);
		void expect(char c);
	private:
		std::string buf;
		std::vector<virtualServer> serve_confs;
		size_t idx;

		void parseServe(size_t i);
		Location parseLocation();
		std::string getToken(char delimiter);
		std::string get_token_to_eol();
};

//static std::vector<std::string> methodsSplit(std::string strs, char delimi);
//void skip();
const std::string readConfFile(const std::string& file_name);
#endif
