#include <tinyxml2.h>
#include "common/config.h"

namespace rocket {

    static Config* g_config = nullptr;

    Config* Config::GetGlobalConfig() {
        return g_config;
    }

    void Config::SetGlobalConfig(std::string xml_file){
        if(nullptr == g_config) {
            g_config = new Config(xml_file);
        }
    }

    Config::Config(std::string xml_file) : m_log_level("DEBUG"){
        
        //加载xml文件
        tinyxml2::XMLDocument doc;
	    if(tinyxml2::XML_SUCCESS != doc.LoadFile(xml_file.c_str())) {
            printf("load xml_file=%s failed!",xml_file.c_str());
            exit(0);
        }

        // tinyxml2::XMLText* logLevelNode = doc.RootElement()->FirstChildElement( "log")->FirstChildElement("level")->ToText();
        // if(nullptr != logLevelNode) {
        //     m_log_level = logLevelNode->Value();
        // }

        // auto root = doc.RootElement();

        // auto log = root->FirstChildElement("log");

        // auto level = log->FirstChildElement("level");

        // m_log_level = level->GetText();
    }
}