#include <chrono>
#include <sstream>
#include <iostream>
#include "common/log.h"
#include "common/util.h"

namespace rocket {

    static Logger* g_logger = nullptr;

    Logger* Logger::GetGlobalLogger() {
        if(!g_logger) {
            return g_logger;
        }

        return new Logger(LogLevel::UNKNOWN);
    }


    void Logger::pushLog(std::string& str) {
        m_queue.push(str);
    }

    void Logger::log() {
        while(!m_queue.empty()) {
            auto str = m_queue.front();
            m_queue.pop();
            std::cout << str << std::endl;
        }
    }

    std::string LogLevelToString(LogLevel level) {
        switch(level) {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Error:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    std::string LogEvent::toString(){

        m_pid = getPid();
        m_thread_id = getThreadId();

        auto time_point_now = std::chrono::system_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds> (time_point_now.time_since_epoch());
        auto ms = duration_ms.count() % 1000;

        auto local_time_t = std::chrono::system_clock::to_time_t(time_point_now);
        auto local_tm_p = std::localtime(&local_time_t);
        
        char buf[128] = {0};
        strftime(buf,128,"%y-%m-%d %H:%M:%S",local_tm_p);

        std::string time_str(buf);

        time_str = time_str + "." + std::to_string(ms); 

        //组装成指定格式，暂未包含msg
        std::stringstream ss;
        ss  << "["  <<  LogLevelToString(m_level) << "]\t"
            << "["  <<  time_str << "]\t"
            << "["  << m_pid << ":" << m_thread_id << "]\t"
            << "["  <<  __FILE__ << ":" << __LINE__ << "]\t";

        return ss.str();
    }
}


int main() {
        auto str2 = (new rocket::LogEvent(rocket::LogLevel::Debug))->toString();
        std::string event_str = str2 + rocket::formatString("test log %s","11");
        rocket::Logger::GetGlobalLogger()->pushLog(event_str); 
        rocket::Logger::GetGlobalLogger()->log();
}