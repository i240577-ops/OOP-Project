#pragma once
#include <iostream>

class Resource {
private:
    int value;
public:
    Resource(int v = 0) : value(v) {}
    Resource(const Resource& other) {
        this->value = other.value;
    }
    int getValue() const { return value; }
    Resource operator+(const Resource& other) const {
        return Resource(this->value + other.value);
    }
    void operator+=(const Resource& other) {
        this->value += other.value;
    }
}; 
