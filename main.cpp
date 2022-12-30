#include <iostream>
#include "Logger.hpp"

using namespace std;

/*
Logger Test Demo
*/
int main(){

    Log::Logger::getInstance()->open("../today.log");
    Log::Logger::getInstance()->setLevel(Log::Logger::INFO);
    debug("Hello");
    info("info message");
    warn("warn message");
    error("error message");
    fatal("fatal message");
    return 0;
}
