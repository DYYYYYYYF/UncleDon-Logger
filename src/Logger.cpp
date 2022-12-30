#include "Logger.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <sys/_types/_time_t.h>
#include <xlocale/_time.h>

const char* Log::Logger::level[LEVEL_COUNT] = {
    "DEBUG","INFO","WARN","ERROR","FATAL"
};

Log::Logger* Log::Logger::instance = NULL;

Log::Logger* Log::Logger::getInstance(){
    if(instance == NULL) instance = new Log::Logger();
    return instance;
}

void Log::Logger::log(Level level, const char* file, int line, const char* format, ...){
    if(m_level > level) return;
    if(m_os.fail()) throw std::runtime_error("open file failed");
    time_t tick = time(NULL);
    struct tm* ptm = localtime(&tick);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);

    const char* pformat = "%s %s %s:%d";
    int size = snprintf(NULL, 0, pformat, timestamp, Log::Logger::level[level], file, line);
    if(size > 0){
        len += size + 1;
        char* buf = new char[size + 1];
        snprintf(buf, size + 1, pformat, timestamp, Log::Logger::level[level], file ,line);
        buf[size] = '\0';
        m_os << buf;
        delete[] buf;
    }

    va_list valist;
    va_start(valist, format);
    size = vsnprintf(NULL, 0, format, valist);
    va_end(valist);
    if(size > 0){
        len += size + 1;
        char* content = new char[size + 1];
        va_start(valist, format);
        vsnprintf(content, size + 1, format, valist);
        va_end(valist);
        m_os << "\t" << content << "\n";
        delete[] content;
    }
    m_os.flush();
    if(max > 0 && len > max) backup();
}

void Log::Logger::backup(){
    close();
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", ptm);
    std::string tmp = timestamp;
    std::string tfn = m_filename;
    std::string filename = "../" + tfn + tmp;
    if (rename(m_filename, filename.c_str()) != 0) throw std::runtime_error("rename log file failed: "); 
    open(m_filename); 
}

void Log::Logger::setLevel(Log::Logger::Level level){
    this->m_level = level;
}

void Log::Logger::open(const std::string filename){
    this->m_filename = filename.c_str();
    m_os.open(m_filename, std::ios::app);
    if(!m_os.is_open()) throw std::runtime_error("open " + filename + " failed...");
    m_os.seekp(0, std::ios::end);
    len = m_os.tellp();
}

void Log::Logger::close(){
    m_os.close();
}

Log::Logger::Logger(): max(1024), min(0), len(0){

}

Log::Logger::~Logger(){
    close();
}
