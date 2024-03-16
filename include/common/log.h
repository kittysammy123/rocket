#ifndef COMMON_LOG_H
#define COMMON_LOG_H

#include <string>
#include <queue>
#include "common/mutex.h"



#define DEBUGLOG(str,...) \
    if(rocket::Logger::GetGlobalLogger()->getLevel() <= rocket::LogLevel::Debug) { \
        std::string event_str = (new rocket::LogEvent(rocket::LogLevel::Debug,__FILE__,__LINE__))->toString() + rocket::formatString(str,##__VA_ARGS__);\
        rocket::Logger::GetGlobalLogger()->pushLog(event_str); \
        rocket::Logger::GetGlobalLogger()->log(); \
    } \

#define INFOLOG(str,...) \
    if(rocket::Logger::GetGlobalLogger()->getLevel() <= rocket::LogLevel::Info) { \
        std::string event_str = (new rocket::LogEvent(rocket::LogLevel::Info,__FILE__,__LINE__))->toString() + rocket::formatString(str,##__VA_ARGS__);\
        rocket::Logger::GetGlobalLogger()->pushLog(event_str); \
        rocket::Logger::GetGlobalLogger()->log(); \
    } \

#define ERRORLOG(str,...) \
    if(rocket::Logger::GetGlobalLogger()->getLevel() <= rocket::LogLevel::Error) { \
        std::string event_str = (new rocket::LogEvent(rocket::LogLevel::Error,__FILE__,__LINE__))->toString() + rocket::formatString(str,##__VA_ARGS__);\
        rocket::Logger::GetGlobalLogger()->pushLog(event_str); \
        rocket::Logger::GetGlobalLogger()->log(); \
    } \

namespace rocket {

    //日志级别
    enum class LogLevel {
        Unknown = 0,
        Debug = 1,
        Info = 2,
        Error = 3,
    };

    //按照指定格式，格式化字符串
    template<typename... Args>
    std::string formatString(const char* str,Args&&... args) {

        std::string result;

        int size = snprintf(nullptr,0,str,std::forward<Args>(args)...);
        if(size > 0)  {
            result.resize(size);
            snprintf(result.data(),size + 1,str,std::forward<Args>(args)...);
        }

        return result;
    }

    //日志事件
    class LogEvent {
    public:
        LogEvent(LogLevel level,std::string file_name,size_t line) {
            m_level = level;
            m_file_name = file_name;
            m_line = line;
        }

        std::string getFileName() const {
            return m_file_name;
        }

        std::string toString();
    private:
        std::string m_file_name;
        size_t m_line;
        int m_pid;
        int m_thread_id;

        LogLevel m_level; //日志级别
    };

    class Logger {
    public:
        Logger(LogLevel level) {
            m_set_level = level;
        }
        void pushLog(std::string& str);
        void log();

        static Logger* GetGlobalLogger();
        static void InitGlobalLogger();

        LogLevel getLevel(){
            return m_set_level;
        }
    private:
        LogLevel m_set_level;

        std::queue<std::string> m_queue;
        Mutex   m_mutex;
    };
}





#endif