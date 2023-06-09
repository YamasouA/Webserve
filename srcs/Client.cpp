#include "Client.hpp"
#include "http/HttpRes.hpp"

//void Client::set_httpReq(httpReq httpReq){
//	this->httpReq = httpReq;
//}

Client::Client()
{}

Client::Client(const Client& source)
:httpreq(source.get_httpReq()),
    httpres(source.get_httpRes()),
    vServer(source.get_vServer())
    //fd(source.get_fd())
{
	this->fd = source.get_fd();
}

Client& Client::operator=(const Client& rhs)
{
    if (this == &rhs) {
        return *this;
    }
    httpreq = rhs.get_httpReq();
    httpres = rhs.get_httpRes();
    vServer = rhs.get_vServer();
    fd = rhs.get_fd();
    return *this;
}

Client::~Client()
{}


//void Client::set_parsedReq(httpParser parsed_req)
void Client::set_httpReq(httpReq httpreq)
{
    this->httpreq = httpreq;
}

void Client::set_fd(int fd) {
	this->fd = fd;
}

void Client::set_httpRes(HttpRes httpres){
	this->httpres = httpres;
}
void Client::set_vServer(const virtualServer& vServer){
	this->vServer = vServer;
}

int Client::get_fd() const{
	return fd;
}

//httpReq Client::get_httpReq(){
//	return httpReq;
//}

//httpParser Client::get_parsedReq() const
httpReq Client::get_httpReq() const
{
    return httpreq;
}

HttpRes Client::get_httpRes() const {
	return httpres;
}

//HttpRes* Client::get_httpResp() const {
//    return httpres;
//}

virtualServer Client::get_vServer() const{
	return vServer;
}
