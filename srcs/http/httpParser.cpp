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


