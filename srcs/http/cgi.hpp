#ifndef CGI_HPP
#define CGI_HPP

#include "httpReq.hpp"

class Cgi {
    public:
        Cgi();
        Cgi(const Cgi& src);
        Cgi& operator=(const Cgi& rhs);
        ~Cgi();

    prveate:
        httpReq httpreq;
        std::map<std::string, std::string> meta_var; // or sep all var
};

#endif
