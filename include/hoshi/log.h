#ifndef HOSHILOG_H
#define HOSHILOG_H

#include "os.h"

typedef enum
{
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4
} LogLevel;

static inline const char *log_level_str(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_ERROR:
        return "ERROR";
    case LOG_LEVEL_WARN:
        return "WARN";
    case LOG_LEVEL_INFO:
        return "INFO";
    case LOG_LEVEL_DEBUG:
        return "DEBUG";
    default:
        return "LOG";
    }
}

// Enable/disable logging globally
#ifdef LOG_LEVEL

#define LOG(level, fmt, ...)                                                       \
    do                                                                             \
    {                                                                              \
        if ((level) <= LOG_LEVEL)                                                  \
        {                                                                          \
            if (level < LOG_LEVEL_DEBUG)                                           \
            {                                                                      \
                OSReport("[%s] " fmt "\n",                                         \
                         log_level_str(level), ##__VA_ARGS__);                     \
            }                                                                      \
            else                                                                   \
            {                                                                      \
                OSReport("[%s] %s:%d: " fmt "\n",                                  \
                         log_level_str(level), __FILE__, __LINE__, ##__VA_ARGS__); \
            }                                                                      \
        }                                                                          \
    } while (0)

#define LOG_INFO(fmt, ...) LOG(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)

#else

#define LOG_INFO(fmt, ...) ((void)0)
#define LOG_WARN(fmt, ...) ((void)0)
#define LOG_ERROR(fmt, ...) ((void)0)
#define LOG_DEBUG(fmt, ...) ((void)0)

#endif

#endif // LOG_H
