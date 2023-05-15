#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

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
    private:
        std::string buf;
        size_t idx;

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
