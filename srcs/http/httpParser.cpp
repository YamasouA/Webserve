#include "httpParser.hpp"

httpParser::httpParser()
{}

httpParser::httpParser(const httpParser& src)
{
    (void)src;
}

httpParser& httpParser::operator=(const httpParser& rhs)
{
    if (this == &rhs) {
        return *this;
    }
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
