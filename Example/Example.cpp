#include "Logger.hpp"

using namespace std;

/*
Logger Test Demo
*/
int main(){
    // 基础信息配置
    Log::Logger::getInstance()->open("log");                                // 设置日志输出文件名称
    Log::Logger::getInstance()->setLevel(Log::Logger::INFO);                // 设置日志等级
	Log::Logger::getInstance()->setMaxSize(1024);                           // 设置单文件最大存储量
    Log::Logger::getInstance()->SetMode(Log::Logger::eMode_Complex);        // 设置日志输出模式

    // 打印式输出
	UL_DEBUG("Hello");
    UL_INFO("info message");
    UL_WARN("warn message");
    UL_ERROR("error message");
	UL_FATAL("fatal message");

    // 流式输出
    std::string str = "output";
	LOG_DEBUG   << "stream log: " << "debug";
	LOG_INFO    << "stream log: " << "info";
	LOG_WARN    << "stream log: " << "warn";
	LOG_ERROR   << "stream log: " << "error";
    LOG_FATAL   << "stream log: " << "fatal";

#define CORE_LOG UL_FATAL
	// 宏定义
	CORE_LOG("Core log");

    return 0;
}
