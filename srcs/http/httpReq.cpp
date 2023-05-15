#include "httpReq.hpp"

httpReq::httpReq()
{}

httpReq::httpReq(const httpReq& src)
:name(src.getName()),
    value(src.getValue())
{}

httpReq& httpReq::operator=(const httpReq& rhs)
{
    if (this == &rhs) {
        return *this;
    }
    this->name = rhs.getName();
    this->value = rhs.getValue();
    return *this;
}

httpReq::~httpReq()
{}

void httpReq::setName(const std::string& token)
{
    this->name = token;
}

void httpReq::setValue(const std::string& token)
{
    this->value = token;
}

std::string httpReq::getName() const
{
    return this->name;
}

std::string httpReq::getValue() const
{
    return this->value;
}
