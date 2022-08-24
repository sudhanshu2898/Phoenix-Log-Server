#pragma once
#if !defined(UTILITY)
#define UTILITY

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

class Utility{
    public: 
    std::string convertToUpperCase(std::string input){
        std::string response = "";
        for(int i=0; i<input.length(); i++){
            response += toupper(input[i]);
        }
        return response;
    }

    std::string convertToLowerCase(std::string input){
        std::string response = "";
        for(int i=0; i<input.length(); i++){
            response += tolower(input[i]);
        }
        return response;
    }

    std::string configData(){
        std::fstream configFile;
        configFile.open("./config.json", std::ios_base::in);
        std::string config = "", thisLine;
        while (getline(configFile, thisLine)){
            config.append(thisLine);
        }
        configFile.close();
        return config;
    }

    std::string generateUniqueID(){
        const int LENGTH = 16;
        const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyz";
        std::stringstream response;
        for (int i=0; i<LENGTH; i++){
            response<<alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        const auto p1 = std::chrono::system_clock::now();
        long int time = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        response<<time;
        std::string temp;
        response>>temp;
        return temp;
    }
};

#endif