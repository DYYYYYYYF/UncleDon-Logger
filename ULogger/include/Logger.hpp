#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>
#include <sstream>
#include <mutex>

#ifdef _WIN32

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <sys/utime.h>
#include <time.h>
#endif

#ifdef __APPLE__
#include <sys/_types/_time_t.h>
#include <xlocale/_time.h>
#endif

#ifdef BUILD_SHARED_LIB
#ifdef DEXPORT
// Export
#ifdef _MSC_VER
#define EXPORT_UL_DLL __declspec(dllexport)
#else
#define EXPORT_UL_DLL __attribute__((visibility("default")))
#endif

// Import
#else
#ifdef _MSC_VER
#define EXPORT_UL_DLL __declspec(dllimport)
#else
#define EXPORT_UL_DLL
#endif
#endif	// #ifdef DEXPORT

#else
#define EXPORT_UL_DLL
#endif 

namespace Log{

#ifndef LOG_DEBUG
#define LOG_DEBUG  Log::Logger::getInstance()->log(Log::Logger::DEBUG, __FILE__, __LINE__)
#endif

#ifndef LOG_WARN
#define LOG_WARN  Log::Logger::getInstance()->log(Log::Logger::WARN, __FILE__, __LINE__)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR  Log::Logger::getInstance()->log(Log::Logger::ERROR, __FILE__, __LINE__)
#endif

#ifndef LOG_FATAL
#define LOG_FATAL  Log::Logger::getInstance()->log(Log::Logger::FATAL, __FILE__, __LINE__)
#endif

#ifndef LOG_INFO
#define LOG_INFO  Log::Logger::getInstance()->log(Log::Logger::INFO, __FILE__, __LINE__)
#endif

#ifndef UL_DEBUG
#define UL_DEBUG(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif  //UL_DEBUG

#ifndef UL_WARN
#define UL_WARN(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif

#ifndef UL_ERROR
#define UL_ERROR(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif

#ifndef UL_FATAL
#define UL_FATAL(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif

#ifndef UL_INFO
#define UL_INFO(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif

    class Logger{
        public:
            enum Level{
                DEBUG = 0,
                INFO,
                WARN,
                ERROR,
                FATAL,
                LEVEL_COUNT
            };

            enum LogMode {
                eMode_Only_Message = 0x00,
                eMode_Complex = 0x01,
                eMode_Simple = 0x02
            };

	    public:
		    // 流式日志接口
		    class LogStream {
			public:
				LogStream(Logger* logger, Level level, const char* file, int line)
					: logger_(logger), level_(level), file_(file), line_(line) {}

				~LogStream() {
					logger_->log(level_, file_, line_, stream_.str());
				}

				template<typename T>
				LogStream& operator<<(const T& msg) {
					stream_ << msg;
					return *this;
				}

			private:
				Logger* logger_;
				Level level_;
				const char* file_;
				int line_;
				std::ostringstream stream_;
		    };

		    // 流式日志宏的辅助函数
		    LogStream log(Level level, const char* file, int line) {
			    return LogStream(this, level, file, line);
		    }   

            EXPORT_UL_DLL static Logger* getInstance();

			EXPORT_UL_DLL void open(const std::string filename, std::ios::openmode = std::ios::app);
			EXPORT_UL_DLL void log(Level level, const char* file, int line, const char* format, ...);
			EXPORT_UL_DLL void log(Level level, const char* file, int line, const std::string& msg);
			EXPORT_UL_DLL void close();

			EXPORT_UL_DLL void setLevel(Level level);
			EXPORT_UL_DLL void setMaxSize(int max_size) { content_max = max_size; }

            EXPORT_UL_DLL void SetMode(LogMode mode) { m_mode = mode; }

            EXPORT_UL_DLL void PrintTerminalMsg(Level level, const char* msg);

        private:
            Logger();
            ~Logger();
            void backup();

        private:
            static Logger* instance;
            std::mutex log_mutex;
            std::ofstream m_os;
            std::string m_file;
            std::string m_filename;
            static const char* level[LEVEL_COUNT];
            Level m_level;
            LogMode m_mode;
            int content_min, content_max, context_len;

    };
} 
