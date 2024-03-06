#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#include <string>

namespace rocket {
    class Config {
    public:
        static Config* GetGlobalConfig();
        static void SetGlobalConfig(std::string xml_file);

        std::string getLogLevel() {
            return m_log_level;
        }
    private:
        Config(std::string xml_file);
    private:
        std::string m_log_level;
    };
}

#endif