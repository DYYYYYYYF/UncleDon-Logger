#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdarg>

namespace Log{

#define debug(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define warn(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define error(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define fatal(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define info(format, ...) \
    Log::Logger::getInstance()->log(Log::Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

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
            void log(Level level, const char* file, int line, const char* format, ...);
            static Logger* getInstance();
            void open(const std::string filename);
            void close();
            void setLevel(Level level);

        private:
            Logger();
            ~Logger();
            void backup();

        private:
            static Logger* instance;
            std::ofstream m_os;
            const char* m_filename;
            static const char* level[LEVEL_COUNT];
            Level m_level;
            int min, max, len;

    };
} 
