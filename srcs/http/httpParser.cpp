#include "httpParser.hpp"

httpParser::httpParser()
{}

httpParser::httpParser(const std::string& request_msg)
:buf(request_msg),
    idx(0)
{}

httpParser::httpParser(const httpParser& src)
:method(src.getMethod()),
    uri(src.getUri()),
    version(src.getVersion()),
    header_info(src.getHeaderInfo()),
    content_body(src.getContetBody())
{
    (void)src;
}

httpParser& httpParser::operator=(const httpParser& rhs)
{
    if (this == &rhs) {
        return *this;
    }
    this->method = rhs.getMethod();
    this->uri = rhs.getUri();
    this->version = rhs.getVersion();
    this->header_info = rhs.getHeaderInfo();
    this->content_body = rhs.getContetBody();
    return *this;
}

httpParser::~httpParser()
{}

void httpParser::setMethod(const std::string& token)
{
    this->method = token;
}

void httpParser::setUri(const std::string& token)
{
    this->uri = token;
}

void httpParser::setVersion(const std::string& token)
{
    this->version = token;
}

void httpParser::setContentBody(const std::string& token)
{
    this->content_body = token;
}

std::string httpParser::getMethod() const
{
    return this->method;
}

std::string httpParser::getUri() const
{
    return this->uri;
}

std::string httpParser::getVersion() const
{
    return this->version;
}

std::string httpParser::getContetBody() const
{
    return this->content_body;
}

std::vector<httpReq> httpParser::getHeaderInfo() const
{
    return this->header_info;
}

void httpParser::skipSpace()
{
	while (buf[idx] == ' ' || buf[idx] == '\t') {
		++idx;
	}
}

void httpParser::trim(std::string& str)
{
	std::string::size_type left = str.find_first_not_of("\t ");
	if (left != std::string::npos) {
		std::string::size_type right = str.find_last_not_of("\t ");
		str = str.substr(left, right - left + 1);
	}
}

void httpParser::expect(char c)
{
    if (buf[idx] != c) {
        std::cerr << "no expected " << c << std::endl;
    }
    ++idx;
}

std::string httpParser::getToken(char delimiter)
{
	std::string token = "";
	while(idx < buf.length()) {
		if (buf[idx] == delimiter) {
			break;
		}
		token += buf[idx];
		idx++;
	}
	if (idx == buf.length()) {
		std::cout << "delimiter: " << delimiter << std::endl;
		std::cout << "token: " << token<< std::endl;
		std::cout << "ko getToken" << std::endl;
		throw SyntaxException("syntax error in getToken");
	}
	expect(delimiter);
	trim(token);
	return token;
}

std::string httpParser::getToken_to_eol() {
	std::string line = "";
	while (idx < buf.length()) {
		if (buf[idx] == '\015') {
			if (buf[idx+1] == '\012') {
				idx += 2;
				return line;
			}
		} else if (buf[idx] == '\012') {
			idx++;
			return line;
		}
		line += buf[idx];
		idx++;
	}
	return line;
}

std::string httpParser::getToken_to_eof() {
	std::string body = "";
	while (idx < buf.length()) {
		body += buf[idx];
		idx++;
	}
	return body;
}

void httpParser::parseReqLine()
{
    method = getToken(' ');
    skipSpace();
    uri = getToken(' ');
    skipSpace();
    version = buf.substr(idx, 8);
    idx += 8;
    if (version != "HTTP/1.1") { //tmp fix version
        std::cerr << "version error" << std::endl;
    }
    if (buf[idx] == '\015') {
        ++idx;
        expect('\012');
    } else if (buf[idx] == '\012') {
        ++idx;
    } else {
        std::cerr << "invalid format" << std::endl;
    }
}

bool httpParser::checkHeaderEnd()
{
    if (buf[idx] == '\015') {
        ++idx;
        expect('\012');
        return true;
    } else if (buf[idx] == '\012') {
        ++idx;
        return true;
    } else {
        return false;
    }
}

void httpParser::parseRequest()
{
   parseReqLine();
   std::cout << "req line ok" << std::endl;
    while (idx < buf.size()) {
        if (checkHeaderEnd()) {
            break;
        }
        httpReq header_field;
        header_field.setName(getToken(':'));
        skipSpace(); //
		std::string s = getToken_to_eol();
		trim(s);
        header_field.setValue(s);
        header_info.push_back(header_field);
    }
    content_body = getToken_to_eof();
}

std::ostream& operator<<(std::ostream& stream, const httpParser& obj) {
    const std::vector<httpReq> tmp = obj.getHeaderInfo();
    stream << "method: " << obj.getMethod() << std::endl
    << "uri: " << obj.getUri() << std::endl
    << "version" << obj.getVersion() << std::endl << std::endl;
    for (std::vector<httpReq>::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
        stream << "header field: " << (*it).getName() << std::endl
        << "value: " << (*it).getValue() << std::endl;
    }
    stream << std::endl
    << "body: " << obj.getContetBody() << std::endl;
    return stream;
}

httpParser::SyntaxException::SyntaxException(const std::string& what_arg)
:msg(what_arg)
{}

httpParser::SyntaxException::~SyntaxException() throw()
{}

const char* httpParser::SyntaxException::what(void) const throw() //noexcept c++11~
{
	return msg.c_str();
}
