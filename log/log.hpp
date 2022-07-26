#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>

using namespace std;

#define WRITE_APPEND            "a"
#define WRITE_OVER              "w"

#define WRITE_MODE              WRITE_OVER

typedef enum
{
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
} log_type_t;

typedef enum 
{
    CHECKSIZE_SPACELEFT = 0,
    CHECKSIZE_OVERFLOW = 1,
} check_size_t;

string logInit(const string &file);
void logSetThreadName (const string &threadName);
void log(log_type_t type, const char * file, int line, const char * format, ...);
#define LOG(type, ...)      log(type, __FILE__, __LINE__, __VA_ARGS__)


#endif /* __LOG_HPP__ */
