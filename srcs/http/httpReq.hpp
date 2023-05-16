#ifndef HTTPREQ_HPP
#define HTTPREQ_HPP

#include <string>

class Client;

class httpReq { //httpReqHeaders is better?
    public:
        httpReq();
        httpReq(const httpReq& src);
        httpReq& operator=(const httpReq& rhs);
        ~httpReq();

        void setName(const std::string& token);
//        void setName_len(std::string);
        void setValue(const std::string& token);
//        void setValue_len(std::string);
        std::string getName() const;
//        size_t getName_len() const;
        std::string getValue() const;
//        size_t getValue_len() const;
    private:
        std::string method;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> header_fields;
        std::string content_body;

//        std::string name;
//        size_t name_len; //neccesary?
//        std::string value;
//        size_t value_len;
};

#endif
