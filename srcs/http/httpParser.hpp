#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include "httpReq.hpp"

class httpParser {
    public:
        httpParser(const std::string& request_msg);
        httpParser(const httpParser& src);
        httpParser& operator=(const httpParser& rhs);
        ~httpParser();


        void setMethod(const std::string&);
        void setUri(const std::string&);
        void setVersion(const std::string&);
        void setContentBody(const std::string&);

        std::string getMethod() const;
        std::string getUri() const;
        std::string getVersion() const;
        std::string getContetBody() const;
        std::vector<httpReq> getHeaderInfo() const;
		void parseRequest();
    private:
        std::string buf;
        size_t idx;

        std::string method;
        std::string uri;
        std::string version;
        std::vector<httpReq> header_info;

		void trim(std::string& str);
		void skipSpace();
		void expect(char c);
		std::string getToken(char delimiter);
		std::string getToken_to_eol();
		void parseReqLine();
		bool checkHeaderEnd();
		std::string getToken_to_eof();
//        std::string method;
//        std::string uri;
//        std::string version;
//        std::string user_agent;
//        std::string accept_language;
//        std::string accept_encoding;
//        std::string connection;
//        std::string upgrade_insecure_req;
//        std::string content_type;
//        size_t content_length;
        std::string content_body;
		class SyntaxException: public std::exception {
			public:
				explicit SyntaxException(const std::string& what_arg);
				~SyntaxException() throw();
				virtual const char* what() const throw(); // throw() = noexcept
			private:
				std::string msg;
		};
};

std::ostream& operator<<(std::ostream& stream, const httpParser& obj);

#endif
