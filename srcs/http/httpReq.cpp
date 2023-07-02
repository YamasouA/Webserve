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
    content_body(src.getContentBody()),
	parse_error(false)
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
{
}

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

int httpReq::getContentLength() const
{
    return this->content_length;
}

int httpReq::getKeepAlive() const
{
    return this->keep_alive;
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

bool httpReq::isSpace(char c) {
	if (c == '\f' || c == '\n' || c == ' '
		|| c == '\r' || c == '\t' || c == '\v') {
		return true;
	}
	return false;
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
    if (token.find(' ') != std::string::npos) {
        std::cerr << "status 400" << std::endl;
    }
//	trim(token);
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

void httpReq::checkUri() {
	std::string::size_type query_pos = uri.find("?");
    std::string::size_type fragment_pos = uri.find("#");
	if (query_pos == std::string::npos) {
		return;
	}
    if (fragment_pos == std::string::npos) {
	    args = uri.substr(query_pos+1);
    } else {
        args = uri.substr(query_pos + 1, fragment_pos);
//        fragment = uri.substr(fragment_pos + 1);
    }
    uri = uri.substr(0, query_pos);
}

void httpReq::parse_scheme() {
	if (uri.compare(0, 5, "https") == 0) {
        uri = uri.substr(6);
//        scheme = HTTPS;
	} else if (uri.compare(0, 4, "http") == 0) {
        uri = uri.substr(5);
//        scheme = HTTP;
	} else {
        std::cerr << "invalid scheme" << std::endl;
	}
}

void httpReq::parse_host_port() {
    std::string host;
	size_t i = 0;
	size_t path_pos;
	std::string port_str;

    for (; uri[i]; ++i) {
        host += uri[i];
        if (uri[i] == ':' || uri[i] == '/') {
            break;
        }
    }
    if (host.length() <= 0) {
        std::cerr << "invalid host" << std::endl;
    }
    if (uri[i] == ':') {
        path_pos = uri.find('/');
        i = path_pos;
        //if (port_pos != std::string::npos) {
        if (path_pos != std::string::npos) {
            port_str = uri.substr(i + 1, path_pos);
            std::stringstream ss(port_str);
            int port_num;
            ss >> port_num;
			if (port_num < 0 || 65535 < port_num) {
        	    std::cerr << "invalid port" << std::endl;
        	}
        }
    }
    if (uri[i] != '/') {
        std::cerr << "path not found" << std::endl;
    }
    //path handle ...
	uri = uri.substr(i);
	checkUri();


    // :/が見つかるまでをhostとして切り取る :が見つかった場合はportの処理も行う
    // hostの長さが0で無いかのチェックとport番号が有効かのチェックを行う
    // host以降の始めが/だった場合uri(request-target)として切り取る checkuri呼べば良さそう?
    // host以降の始めが/ではなかった場合invalid format
}

void httpReq::parse_authority_and_path() {
	parse_host_port(); //関数に分けなくても良い?
}

void httpReq::absurl_parse() {
	parse_scheme();
	expect('/');
	expect('/');
	parse_authority_and_path();
}

void httpReq::fix_up() {
	if (header_fields.count("host") != 1) {
		std::cerr << "status " << std::endl;
	}

	if (header_fields.count("connection") != 1) {
		std::cerr << "status " << std::endl;
	}
    if (header_fields["connection"] == "keep-alive") {
        keep_alive = 1;
    } else {
        keep_alive = 0;
    }
	if (header_fields.count("content_length") != 1) {
		std::cerr << "status " << std::endl;
	}
	std::string content_length_s = header_fields["content_length"];
    std::stringstream ss(content_length_s);
    ss >> content_length;

	if (!(method == "GET" || method == "DELETE" || method == "POST")) {
		std::cerr << "status " << std::endl;
	}
	if (uri.length() != 0 && uri[0] != '/') {
		absurl_parse();
	}
}

void httpReq::parseReqLine()
{
    method = getToken(' ');
    if (isSpace(buf[idx])) {
        std::cerr << "status 400" << std::endl;
    }
//    skipSpace();
    uri = getToken(' ');
	checkUri();
	if (uri.length() != 0 && uri[0] != '/') {
		absurl_parse();
	}
    if (isSpace(buf[idx])) {
        std::cerr << "status 400" << std::endl;
    }
//    skipSpace();
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

std::string httpReq::toLower(std::string str) {
	std::string s="";
	for (size_t i = 0; i < str.length(); i++) {
		s += std::tolower(str[i]);
	}
	return s;
}

bool httpReq::hasObsFold(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		// OWS CRLF RWS
		if (str[i] == '\015' && str[i + 1] && str[i + 1] == '\012') {
			if (str[i + 2] && isSpace(str[i + 2])) {
				return true;
			}
		}
	}
	return false;
}

static bool isVCHAR(std::string str) {
    for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it) {
        if (*it <= 32 && 127 <= *it) {
            return false;
        }
    }
    return true;
}

void httpReq::checkFieldsValue() {
	for (std::map<std::string, std::string>::iterator it = header_fields.begin();
		it != header_fields.end(); it++) {
		if (hasObsFold(it->second)) {
			parse_error = true;
			return;
		}
        if (!(isVCHAR(it->second))) {
            parse_error = true;
            return;
        }
	}
}

//void httpReq::checkURI() {
//    std::string valid_symbol("-.~:@!$'()");
//    for (std::string::const_iterator it = uri.cbegin(); it != uri.cend(); ++it) {
//        if (!(std::isalnum(*it)) && valid_symbol.find(*it) == std::string::npos) {
//            parse_error = true;
//            return;
//        }
//    }
//}

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
        setHeaderField(toLower(field_name), field_value);
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
