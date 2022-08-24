#pragma once
#if !defined(MESSAGINGQUEUE)
#define MESSAGINGQUEUE

#include <iostream>
#include <queue>
#include "logGenerator.hpp"

class MessagingQueue{
    
    private:
        LogGenerator logGenerator;
        std::map<std::string, std::string> prevBytes;
        std::queue<std::pair<std::string, std::string>> currentQueue;
        /* In Pair First value is service Name, Second is JSON String. */
    
    public:

        void pushToQueue(std::string service, std::string message){
            if(prevBytes.find(service) != prevBytes.end()){
                if(message[0] != '{'){
                    message = prevBytes[service]+message;
                }
                prevBytes.erase(service);
            }
            int position = 0;  
            std::string token;
            while ((position = message.find ("}{")) != std::string::npos){  
                token = message.substr(0, position+1);
                currentQueue.push({service, token});
                message.erase(0, position + 1);
            }
            if(message[0] == '{' && message[message.length()-1] == '}'){            
                currentQueue.push({service, message});
            }else{
                prevBytes[service] = message;
            }
        }


        void processQueue(){
            while(true){
                if(currentQueue.size() > 0){
                    std::pair<std::string, std::string> currentObject = currentQueue.front();
                    logGenerator.generateLog(currentObject.first, currentObject.second);
                    currentQueue.pop();
                }
            }
        }
};

#endif

