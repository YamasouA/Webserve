#ifndef CONFIGPASER_HPP
#define CONFIGPASER_HPP

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <exception>
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
//		std::map<std::string> directive_map; //neccesary?
		std::string buf;
		std::vector<virtualServer> serve_confs;
		size_t idx;

		void parseServe(size_t i);
		Location parseLocation();
		std::string getToken(char delimiter);
		std::string get_token_to_eol();
		// シンタックスエラー
		class SyntaxException: public std::exception {
			public:
				virtual const char* what() const throw(); // throw() = noexcept
		};
		// 設定重複エラー
		class DupulicateException: public std::exception {
			public:
				virtual const char* what() const throw(); // throw() = noexcept
		};
		//
};

//static std::vector<std::string> methodsSplit(std::string strs, char delimi);
//void skip();
const std::string readConfFile(const std::string& file_name);
#endif
