#include "error.h"

using namespace std;

Error::Error(const std::string& msg)
    : msg("(Luigi) " + msg)
{}

const char *Error::what() const noexcept
{
    return msg.c_str();
}
