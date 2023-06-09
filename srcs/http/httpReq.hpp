#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include <sstream>

class httpReq {
    public:
        httpReq();
        httpReq(const std::string& request_msg);
        httpReq(const httpReq& src);
        httpReq& operator=(const httpReq& rhs);
        ~httpReq();


        void setMethod(const std::string&);
        void setUri(const std::string&);
        void setVersion(const std::string&);
        void setContentBody(const std::string&);
		void setHeaderField(const std::string& name, const std::string value);

        std::string getMethod() const;
        std::string getUri() const;
        std::string getVersion() const;
        std::string getContentBody() const;
        int getContentLength() const;
        std::map<std::string, std::string> getHeaderFields() const;
        int getKeepAlive() const;
		void parseRequest();
        bool isSpace(char c);
		std::string toLower(std::string str);
		int content_length;
    private:
        std::string buf;
        size_t idx;

        std::string method;
        std::string uri;
        std::string args;
        std::string version;
        std::map<std::string, std::string> header_fields;
        std::string content_body;
		bool parse_error;
        int keep_alive;

		void trim(std::string& str);
		void skipSpace();
		void expect(char c);
		std::string getToken(char delimiter);
		std::string getToken_to_eol();
		void parseReqLine();
		bool checkHeaderEnd();
		std::string getToken_to_eof();
		void checkUri();
		void parse_scheme();
		void parse_host_port();
		void checkFieldsValue();
		bool hasObsFold(std::string str);
		void fix_up();
		void absurl_parse();
		void parse_authority_and_path();
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
		class SyntaxException: public std::exception {
			public:
				explicit SyntaxException(const std::string& what_arg);
				~SyntaxException() throw();
				virtual const char* what() const throw(); // throw() = noexcept
			private:
				std::string msg;
		};
};

std::ostream& operator<<(std::ostream& stream, const httpReq& obj);

#endif
