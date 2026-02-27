#pragma once
#include <exception>
#include <string>

class GameException : public std::exception {
private:
    string message;
public:
    GameException(const string& msg) : message(msg) {}
    virtual const char* what() const throw() {
        return message.c_str();
    };
}; 