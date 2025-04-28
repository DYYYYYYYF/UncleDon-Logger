#include "Logger.hpp"

#include <thread>

void ProcessTask1() {
	for (int i = 0; i < 10; i++) {
		LOG_INFO << "Task1, progress: " << i;
	}
}

void ProcessTask2() {
	for (int i = 0; i < 10; i++) {
		LOG_INFO << "Task2, progress: " << i;
	}
}

/*
Logger Test Demo
*/
int main() {
	Log::Logger::getInstance()->open("muti-thread-log");                            
	Log::Logger::getInstance()->setMaxSize(10240);                            

	std::thread thread1 = std::thread(ProcessTask1);
	std::thread thread2 = std::thread(ProcessTask2);

	thread1.join();
	thread2.join();

	return 0;
}
