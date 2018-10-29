//
// Created by Rafiq on 2018-10-28.
//

#ifndef RENDERER_PARSINGEXCEPTION_H
#define RENDERER_PARSINGEXCEPTION_H

#include <exception>
#include <string>

class ParsingException : public std::exception {
    std::string exception_str;
public:
    ParsingException(const std::string &s): exception_str(s) {}
    ~ParsingException() throw () {}
    const char* what() const throw() override { return exception_str.c_str(); }
};

#endif //RENDERER_PARSINGEXCEPTION_H
