#include "Logger.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif

const char* Log::Logger::level_string[LEVEL_COUNT] = {
    "DEBUG","INFO","WARN","ERROR","FATAL"
};

Log::Logger* Log::Logger::instance = NULL;

Log::Logger* Log::Logger::getInstance(){
    if(instance == NULL) instance = new Log::Logger();
    return instance;
}

void Log::Logger::log(Level level, const char* file, int line, const char* format, ...){
    if(m_level > level) return;
    if(m_os.fail()) std::runtime_error("open file failed");
    time_t tick = time(NULL);
    struct tm ptm;
#ifdef _WIN32
    localtime_s(&ptm, &tick);
#elif __APPLE__
    localtime_r(&tick, &ptm);
#endif
  
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &ptm);
	
    log_mutex.lock();
    int size = 0;
    if (m_mode & Log::Logger::LogMode::eMode_Complex) {
		const char* pformat = "%s %s %s:%d";
		size = snprintf(NULL, 0, pformat, timestamp, Log::Logger::level_string[level], file, line);
		if (size > 0) {
			context_len += size + 1;
			char* buf = new char[size + 1];
			snprintf(buf, size + 1, pformat, timestamp, Log::Logger::level_string[level], file, line);
			buf[size] = '\0';
			m_os << buf;
			delete[] buf;
		}
    }
    else if (m_mode & Log::Logger::LogMode::eMode_Simple) {
		const char* pformat = "[%s]:";
		size = snprintf(NULL, 0, pformat, Log::Logger::level_string[level]);
		if (size > 0) {
			context_len += size + 1;
			char* buf = new char[size + 1];
            snprintf(buf, size + 1, pformat, Log::Logger::level_string[level]);
			buf[size] = '\0';
			m_os << buf;
			delete[] buf;
		}
    }



    va_list valist;
    va_start(valist, format);
    size = vsnprintf(NULL, 0, format, valist);
    va_end(valist);
    if(size > 0){
        context_len += size + 1;
        char* content = new char[size + 1];
        va_start(valist, format);
        vsnprintf(content, size + 1, format, valist);
        va_end(valist);

        std::string out_msg = std::string(content) + "\n";
        // log file need a tab gap
        m_os << "\t" << out_msg;

        // print to terminal
        PrintTerminalMsg(level, out_msg.c_str());
        delete[] content;
    }
    m_os.flush();
	log_mutex.unlock();

    if(content_max > 0 && context_len > content_max) backup();
}

void Log::Logger::log(Level level, const char* file, int line, const std::string& msg){
    log(level, file, line, msg.c_str());
}

void Log::Logger::backup(){
    close();
    time_t ticks = time(NULL);
    struct tm ptm;

#ifdef _WIN32
    localtime_s(&ptm, &ticks);
#elif __APPLE__
    localtime_r(&ticks, &ptm);
#endif

    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "-%Y-%m-%d_%H-%M-%S", &ptm);
    std::string tmp = timestamp;
    std::string tfn = m_filename;

    std::string filename = tfn + tmp;
    std::string file = filename + ".log";

    if (rename(m_file.c_str(), file.c_str())) {
        std::cout << "error" << std::endl;
        std::runtime_error("rename log file failed: ");
    } 
    open(tfn, std::ios::ate); 
}

void Log::Logger::setLevel(Log::Logger::Level level){
    this->m_level = level;
}

void Log::Logger::open(const std::string filename, std::ios::openmode is_type){

    std::string file = filename + ".log";
    m_file = file.c_str();
    m_filename = filename.c_str();

    m_os.open(m_file, is_type);
    if(!m_os.is_open()) std::runtime_error("open " + file + " failed...");
    m_os.seekp(0, std::ios::end);
    context_len = (int)m_os.tellp();
}

void Log::Logger::close(){
    m_os.close();
}

Log::Logger::Logger(): content_max(1024), content_min(0), context_len(0){
#ifdef _DEBUG
    m_level = Log::Logger::Level::eDEBUG;
    m_mode = Log::Logger::LogMode::eMode_Simple;

#else
    m_level = Log::Logger::Level::eINFO;
    m_mode = Log::Logger::LogMode::eMode_Complex;

#endif

}

Log::Logger::~Logger(){
    close();
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
void Log::Logger::PrintTerminalMsg(Level level, const char* msg) {
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	/*
    *   低四位：Text   高四位：Background
        0 = 黑色    8 = 灰色    1 = 淡蓝      9 = 蓝色
		2 = 淡绿    A = 绿色    3 = 湖蓝      B = 淡浅绿
		C = 红色    4 = 淡红    5 = 紫色      D = 淡紫
		6 = 黄色    E = 淡黄    7 = 白色      F = 亮白
    */
    static unsigned char levels[6] = {0x02, 0x08, 0x06, 0x04, 0x0C, 1}; // Debug Info Warn Error Fatal
	SetConsoleTextAttribute(ConsoleHandle, levels[level]);
	OutputDebugStringA(msg);
	unsigned long long Length = strlen(msg);
	LPDWORD NumberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, (DWORD)Length, NumberWritten, 0);
	SetConsoleTextAttribute(ConsoleHandle, 8);
}
#elif defined(__APPLE__)
void Log::Logger::PrintTerminalMsg(Level level, const char* msg) {
	// FATAL,ERROR,WARN,INFO,DEBUG,TRACE
	const char* colour_strings[] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;30" };
	printf("\033[%sm%s\033[0m", colour_strings[color], message);
}
#endif