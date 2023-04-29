#include "HttpParser.hpp"

int main() {
	/*
	{
		std::cout << "GET /hoge/fuga HTTP/1.1" << std::endl;
		HttpParser parser(std::string("GET /hoge/fuga HTTP/1.1\r\n"));
		parser.parse_request();
	}
	*/
//	{
//		std::cout << "GET /hoge/fuga HTTP/1.1" << std::endl;
//		std::cout << "content-text: text/html, img" << std::endl;
//		HttpParser parser(std::string("GET /hoge/fuga HTTP/1.1\r\ncontent-text: text/html, img\r\n\r\n"));
//		parser.parse_request();
//	}

	{
		std::cout << "GET /hoge HTTP/1.1" << std::endl;
		std::cout << "Host: example.com\r\nCookie: \r\n\r\n" << std::endl;
		HttpParser parser(std::string("GET /hoge HTTP/1.1\r\nHost: example.com\r\nCookie: \r\n\r\n"));
		parser.parse_request();
	}
//
//	std::cout << "Error case" << std::endl;
//	{
//		std::cout << "" << std::endl;
//		HttpParser parser(std::string(""));
//		parser.parse_request();
//	}
}
