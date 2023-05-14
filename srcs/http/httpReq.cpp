#include "httpReq.hpp"

httpReq::httpReq()
{}

httpReq::httpReq(const httpReq& src)
{
    (void)src;
}

httpReq& httpReq::operator=(const httpReq& rhs)
{
    if (this == &rhs) {
        return *this;
    }
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
