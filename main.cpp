#include <iostream>
#include "Logger.hpp"

using namespace std;

/*
Logger Test Demo
*/
int main(){

    Log::Logger::getInstance()->open("today");
    Log::Logger::getInstance()->setLevel(Log::Logger::INFO);
    DEBUG("Hello");
    INFO("info message");
    WARN("warn message");
    ERROR("error message");
    FATAL("fatal message");
    return 0;
}
