#ifndef CONFIGPASER_HPP
#define CONFIGPASER_HPP

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <exception>
#include "virtualServer.hpp"
#include "Location.hpp"
#include <map>

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
		std::vector<virtualServer> get_serve_confs()const;
		Location get_uri2location(std::string uri) const;
	private:
//		std::map<std::string> directive_map; //neccesary?
		std::string buf;
		std::vector<virtualServer> serve_confs;
		size_t idx;

		//void parseServe(size_t i);
		virtualServer parseServe();
		void setUriToMap(std::string prefix, std::string prefix_root, Location location);
		void uriToMap(virtualServer& vServer);
		Location parseLocation();
		std::string getToken(char delimiter);
		std::string get_token_to_eol();
		std::map<std::string, Location> uri2location;
		// シンタックスエラー
		class SyntaxException: public std::exception {
			public:
				explicit SyntaxException(const std::string& what_arg);
				~SyntaxException() throw();
				virtual const char* what() const throw(); // throw() = noexcept
			private:
				std::string msg;
		};
		// 設定重複エラー
		class DupulicateException: public std::exception {
			public:
				explicit DupulicateException(const std::string& what_arg);
				~DupulicateException() throw();
				virtual const char* what() const throw(); // throw() = noexcept
			private:
				std::string msg;
		};
		//
};

//static std::vector<std::string> methodsSplit(std::string strs, char delimi);
//void skip();
const std::string readConfFile(const std::string& file_name);
#endif
