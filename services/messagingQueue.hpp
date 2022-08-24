#pragma once
#if !defined(MESSAGINGQUEUE)
#define MESSAGINGQUEUE

#include <iostream>
#include <queue>
#include "logGenerator.hpp"

class MessagingQueue{
    
    private:
        std::queue<std::pair<std::string, std::string>> currentQueue;
        LogGenerator logGenerator;
        /**
         * In Pair First value is service Name, Second is JSON String
         */
    
    public:

        void pushToQueue(std::string service, std::string message){
            
            int position = 0;  
            std::string token;
            while ((position = message.find ("}{")) != std::string::npos){  
                token = message.substr(0, position+1);
                currentQueue.push({service, token});
                message.erase(0, position + 1);
            }
            currentQueue.push({service, message});  
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

