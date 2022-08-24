#pragma once
#if !defined(LOGGER)
#define LOGGER

#include <iostream>
#include <fstream>
#include <time.h>
#include <io.h>
#include "simpleJSON.hpp"

class Logger{

    Utility utility;

    std::string logPath;
    std::string formateMessage(std::string message){
        /** Get Current Time */
        time_t dateTimeNow = time(0);
        std::string dateTime = ctime(&dateTimeNow);
        dateTime[dateTime.length() - 1] = '\0';
        /** Fomate the message */
        message = "["+dateTime+"]\t"+message+"\n";
        return message;
    }

    public:
    Logger(std::string serviceName){
        this->logPath = "./logs/"+serviceName;
    }

    void logInfo(std::string message){
        /* WRITE TO INFO LOG FILE */
        std::fstream logFile;
        logFile.open(logPath+"/info.log", std::ios_base::app);
        logFile<<formateMessage(message);
        logFile.close();
    }

    void logError(std::string message){
        /* WRITE TO ERROR LOG FILE */
        std::fstream logFile;
        logFile.open(logPath+"/error.log", std::ios_base::app);
        logFile<<formateMessage(message);
        logFile.close();
    }

    void logStartup(std::string message){
        /* WRITE TO STARTUP LOG FILE */
        std::fstream logFile;
        logFile.open(logPath+"/startup.log", std::ios_base::app);
        logFile<<formateMessage(message);
        logFile.close();
    }


    void logObject(std::string collection, std::string format, json::JSON object){
        format = utility.convertToLowerCase(format);
        std::string collectionPath = logPath+"/"+collection;
        mkdir(collectionPath.c_str());
        collectionPath = collectionPath+"/"+utility.generateUniqueID()+"."+format;
        std::ofstream objectFile(collectionPath);
        if(format == "txt"){
            objectFile<<object.ToString();
        }else{
            objectFile<<object;
        }
        objectFile.close();
    }
};

#endif