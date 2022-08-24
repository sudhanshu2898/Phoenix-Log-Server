#pragma once
#if !defined(CREATELOGFILES)
#define CREATELOGFILES

#include <iostream>
#include <io.h>
#include <fstream>

class CreateLogFiles{
    public:
    CreateLogFiles(std::string serviceName){
        std::ifstream tempFile;
        std::string logFolder = "./logs";
        /**
         * Create logs folder
         */
        mkdir(logFolder.c_str());

        /**
         * Create the Log Files folder for current client
         */
        logFolder = "./logs/"+serviceName;
        mkdir(logFolder.c_str());

        /**
         * Create the Info Log File
         */
        tempFile.open(logFolder+"/info.log");
        if(!tempFile){
            std::ofstream(logFolder+"/info.log");
        }
        tempFile.close();

        /**
         * Create the error Log File
         */
        tempFile.open(logFolder+"/error.log");
        if(!tempFile){
            std::ofstream(logFolder+"/error.log");
        }
        tempFile.close();

        /**
         * Create the startup Log File
         */
        tempFile.open(logFolder+"/startup.log");
        if(!tempFile){
            std::ofstream(logFolder+"/startup.log");
        }
        tempFile.close();
    }
};

#endif