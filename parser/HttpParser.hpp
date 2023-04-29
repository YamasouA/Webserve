#ifndef HTTPPARSER_CPP
#define HTTPPARSER_CPP


#include <iostream>
#include <string>
#include <vector>
#include <map>

/*
typedef struct parse_header parse_header;
struct parse_header {
	char *name;
	char *value;
	size_t name_len;
	size_t value_len;
};
*/

class  HttpParser {
	private:
		// headerの順に意味があるならmapではなくparse_headerのlistとかの方が良さそう
		std::map<std::string, std::string> headers;
		//std::map<std::string, parse_header> headers;
		std::string version;
		std::string method;
		std::string resource;
		std::string rmsg;
		size_t rmsg_len;
		//char *rmsg;

		std::string get_token(char next_char);
		std::string get_token_to_eol();
		std::string trim(std::string src);
		void parse_headers();
		
	public:
		//HttpParser(std::string request);
		HttpParser(std::string request);
		~HttpParser();
		HttpParser(const HttpParser& source);
		HttpParser& operator =(const HttpParser& source);
		std::string getVersion();
		std::string getMethod();
		void parse_request();
};
#endif
