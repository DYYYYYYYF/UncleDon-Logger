# UncleDon-Logger
A Logger with C++  

CMakeFile  

add include_dirctions and lib to yourself project  
Environment: Linux / Unix / Windows

# Method  
#### 打印式输出（支持 %d %s 等格式）：UL_INFO("number: %d", 5)

UL_DEBUG()  
UL_INFO()  
UL_WARN()  
UL_ERROR()  
UL_FATAL()  

#### 流式输出（支持自动推到类型）：LOG_INFO << "number: " << 5

LOG_DEBUG

LOG_INFO

LOG_WARN

LOG_ERROR

LOG_FATAL

# Some Suggestions
before use UncleDon-Logger  

include ./include direction
link lib/${plant_lib}/Logger.*

set first logFilename: `Log::Logger::getInstance()->open("today") `

# CMake && Make  
Single build:
Windows：  
```shell
  CMake .. - G "Visual Studio 16 2019"  
  Generate .sln file under build  
```

Linux/MacOS:  
```shell
  CMake ..  
  make  
```

Add to project:
```cmake
add_subdirectory(UncleDon-Logger)
```