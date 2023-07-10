#ifndef CGI_HPP
#define CGI_HPP

#include <unistd.h>
#include "httpReq.hpp"

class Cgi {
    public:
        Cgi(const httpReq& request);
        Cgi(const Cgi& src);
        Cgi& operator=(const Cgi& rhs);
        ~Cgi();

        void run_cgi();
        char buf[1024];
    private:
        void fork_process();
        void run_handler();
        void send_body_to_child();
        void set_env();
        void fix_up();
        std::string encode_uri();
        bool check_meta_var(std::string var1, std::string var2);
        httpReq httpreq;
        std::map<std::string, std::string> envs; // or sep all var
};

#endif
