#include "Client.hpp"
#include "http/HttpRes.hpp"

//void Client::set_httpReq(httpReq httpReq){
//	this->httpReq = httpReq;
//}

Client::Client()
{}

Client::Client(const Client& source)
:parsed_req(source.get_parsedReq()),
    httpres(source.get_httpResp()),
    vServer(source.get_vServer()),
    fd(source.get_fd())
{
}

Client& Client::operator=(const Client& rhs)
{
    if (this == &rhs) {
        return *this;
    }
    parsed_req = rhs.get_parsedReq();
    httpres = rhs.get_httpResp();
    vServer = rhs.get_vServer();
    fd = rhs.get_fd();
    return *this;
}

Client::~Client()
{}

void Client::set_parsedReq(httpParser parsed_req)
{
    this->parsed_req = parsed_req;
}

void Client::set_fd(int fd) {
	this->fd = fd;
}

void Client::set_httpRes(HttpRes* httpres){
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

httpParser Client::get_parsedReq() const
{
    return parsed_req;
}

HttpRes Client::get_httpRes() const {
	return *httpres;
}

HttpRes* Client::get_httpResp() const {
    return httpres;
}

virtualServer Client::get_vServer() const{
	return vServer;
}
