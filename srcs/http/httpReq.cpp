#include "httpReq.hpp"

httpReq::httpReq()
{}

httpReq::httpReq(const std::string& request_msg)
:buf(request_msg),
    idx(0)
{}

httpReq::httpReq(const httpReq& src)
:method(src.getMethod()),
    uri(src.getUri()),
    version(src.getVersion()),
    header_fields(src.getHeaderFields()),
    content_body(src.getContentBody())
{
    (void)src;
}

httpReq& httpReq::operator=(const httpReq& rhs)
{
    if (this == &rhs) {
        return *this;
    }
    this->method = rhs.getMethod();
    this->uri = rhs.getUri();
    this->version = rhs.getVersion();
    this->header_fields = rhs.getHeaderFields();
    this->content_body = rhs.getContentBody();
    return *this;
}

httpReq::~httpReq()
{}

void httpReq::setMethod(const std::string& token)
{
    this->method = token;
}

void httpReq::setUri(const std::string& token)
{
    this->uri = token;
}

void httpReq::setVersion(const std::string& token)
{
    this->version = token;
}

void httpReq::setContentBody(const std::string& token)
{
    this->content_body = token;
}

void httpReq::setHeaderField(const std::string& name, const std::string value)
{
    this->header_fields.insert(std::make_pair(name, value));
}

std::string httpReq::getMethod() const
{
    return this->method;
}

std::string httpReq::getUri() const
{
    return this->uri;
}

std::string httpReq::getVersion() const
{
    return this->version;
}

std::string httpReq::getContentBody() const
{
    return this->content_body;
}

std::map<std::string, std::string> httpReq::getHeaderFields() const
{
    return this->header_fields;
}
//std::vector<httpReq> httpReq::getHeaderInfo() const
//{
//    return this->header_info;
//}

void httpReq::skipSpace()
{
	while (buf[idx] == ' ' || buf[idx] == '\t') {
		++idx;
	}
}

void httpReq::trim(std::string& str)
{
	std::string::size_type left = str.find_first_not_of("\t ");
	if (left != std::string::npos) {
		std::string::size_type right = str.find_last_not_of("\t ");
		str = str.substr(left, right - left + 1);
	}
}

void httpReq::expect(char c)
{
    if (buf[idx] != c) {
        std::cerr << "no expected " << c << std::endl;
    }
    ++idx;
}

std::string httpReq::getToken(char delimiter)
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

std::string httpReq::getToken_to_eol() {
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

std::string httpReq::getToken_to_eof() {
	std::string body = "";
	while (idx < buf.length()) {
		body += buf[idx];
		idx++;
	}
	return body;
}

void httpReq::parseReqLine()
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

bool httpReq::checkHeaderEnd()
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

void httpReq::parseRequest()
{
   parseReqLine();
   std::cout << "req line ok" << std::endl;
    while (idx < buf.size()) {
        if (checkHeaderEnd()) {
            break;
        }
//        httpReq header_field;
        std::string field_name = getToken(':');
//        header_field.setName(getToken(':'));
        skipSpace(); //
		std::string field_value = getToken_to_eol();
		trim(field_value);
//        header_field.setValue(s);
        setHeaderField(field_name, field_value);
//        header_info.push_back(header_field);
    }
    content_body = getToken_to_eof();
}

std::ostream& operator<<(std::ostream& stream, const httpReq& obj) {
//    const std::vector<httpReq> tmp = obj.getHeaderInfo();
    const std::map<std::string, std::string> tmp = obj.getHeaderFields();
    stream << "method: " << obj.getMethod() << std::endl
    << "uri: " << obj.getUri() << std::endl
    << "version" << obj.getVersion() << std::endl << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
        stream << "header field: " << (*it).first << std::endl
        << "value: " << (*it).second << std::endl;
    }
    stream << std::endl
    << "body: " << obj.getContentBody() << std::endl;
    return stream;
}

httpReq::SyntaxException::SyntaxException(const std::string& what_arg)
:msg(what_arg)
{}

httpReq::SyntaxException::~SyntaxException() throw()
{}

const char* httpReq::SyntaxException::what(void) const throw() //noexcept c++11~
{
	return msg.c_str();
}
