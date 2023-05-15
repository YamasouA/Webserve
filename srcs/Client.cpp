#include "Client.hpp"

void Client::set_httpReq(httpReq httpReq){
	this->httpReq = httpReq;
}

void Client::set_fd(int fd) {
	this->fd = fd;
}

void Client::set_httpRes(httpRes httpRes){
	this->httpRes = httpRes;
}
void Client::set_vServer(virtualServer vServer){
	this->vServer = vServer;
}

int Client::get_fd() {
	return fd;
}

httpReq Client::get_httpReq(){
	return httpReq;
}

httpRes Client::get_httpRes(){
	return httpRes;
}

virtualServer Client::get_vServer(){
	return vServer;
}
