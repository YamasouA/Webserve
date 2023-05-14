#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

class httpParser {
    public:
        httpParser();
        httpParser(const httpParser& src);
        httpParser& operator=(const httpParser& rhs);
        ~httpParser();
    private:
        std::string method;
        std::string uri;
        std::string version;
        std::vector<httpReq> header_info;
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
};

#endif
