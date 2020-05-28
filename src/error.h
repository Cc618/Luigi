#pragma once

// A Luigi exception, Python compatible

#include <exception>
#include <string>

class Error : public std::exception
{
public:
    // Asserts
    static inline void check(bool condition, const std::string& msg)
    {
        if (!condition)
            throw Error(msg);
    }

public:
    explicit Error(const std::string& msg);

public:
    virtual const char *what() const noexcept override;

private:
    std::string msg;
};
