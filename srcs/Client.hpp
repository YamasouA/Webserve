#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "conf/virtualServer.hpp"

#include "http/httpReq.hpp"
#include "http/HttpRes.hpp"
//#include "http/httpParser.hpp" //necessary?

class HttpRes;

class Client {
	private:
		httpReq httpreq;
        //httpParser parsed_req;
		HttpRes *httpres;

		virtualServer vServer;
		int fd;
	public:
		Client();
		~Client();
		Client(const Client& source);
		Client& operator =(const Client& source);

		void set_fd(int fd);

		void set_httpReq(const HttpRes& source);
        //void set_parsedReq(httpParser parsed_req);
        void set_httpReq(httpReq httpreq);
		void set_httpRes(HttpRes* source);
		void set_vServer(const virtualServer& source);
		int get_fd() const;
		//httpParser get_parsedReq() const;
		httpReq get_httpReq() const;
		HttpRes get_httpRes() const;
        HttpRes* get_httpResp() const;
		virtualServer get_vServer() const;

};

#endif
