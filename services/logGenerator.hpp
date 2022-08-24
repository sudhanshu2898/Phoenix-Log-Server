#pragma once
#if !defined(WRITE)
#define WRITE

#include <iostream>
#include <string>
#include "utility.hpp"
#include "logger.hpp"
#include "simpleJSON.hpp"

class LogGenerator{
    Utility utility;
    public:
    void generateLog(std::string serviceName, std::string jsonInput){
        try{
            json::JSON input = json::JSON::Load(jsonInput);
            std::string type = utility.convertToUpperCase(input["type"].ToString());
            Logger logger(serviceName);
            if(type == "INFO_LOG"){
                std::string data = input["data"].ToString();
                logger.logInfo(data);
            }else if(type == "ERROR_LOG"){
                std::string data = input["data"].ToString();
                logger.logError(data);
            }else if(type == "STARTUP_LOG"){
                std::string data = input["data"].ToString();
                logger.logStartup(data);
            }else if(type == "OBJECT"){
                json::JSON data = input["data"];
                std::string collection = input["collection"].ToString();
                std::string format = input["format"].ToString();
                logger.logObject(collection, format, data);
            }
        }catch(...){
            std::cout<<"ERROR PARSING JSON,\n Input Was: "<<jsonInput<<"\n";
        }
    }
};

#endif