#ifndef COMMON_LOG_H
#define COMMON_LOG_H

#include <string>
#include <queue>


#define DEBUGLOG(str,...) \
    std::string event_str = (new rocket::LogEvent(rocket::LogLevel::Debug))->toString() + rocket::formatString(str,##__VA_ARGS__);\
    rocket::Logger::GetGlobalLogger()->pushLog(event_str); \
    rocket::Logger::GetGlobalLogger()->log(); \



namespace rocket {

    //日志级别
    enum class LogLevel {
        UNKNOWN = 0,
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
        LogEvent(LogLevel level) {
            m_level = level;
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
    private:
        LogLevel m_set_level;

        std::queue<std::string> m_queue;
    };
}





#endif