#include <iostream>
#include <map>
#include <winsock2.h>
#include "services/logGenerator.hpp"
#include "services/utility.hpp"
#include "services/createLogFiles.hpp"
#include "services/messagingQueue.hpp"
#include "services/simpleJSON.hpp"
#include "mingwExtra/mingw.thread.h"

using namespace std;
MessagingQueue messagingQueue;

void startMessagingQueuePorcess(){
    messagingQueue.processQueue();
}

int main(){

    /**
     * Start processing messageing queue on another Thread.
     */
    std::thread processThread(startMessagingQueuePorcess);

    /*
        Read data from config file. 
        1. crate a server
        2. Accept connection request
        3. ask service name user
        4. read message
        5. log to file
    */

    std::map<int, string> currentClients;
    Utility utility;
    json::JSON config = json::JSON::Load(utility.configData());
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddress;
                serverAddress.sin_family = AF_INET;
                serverAddress.sin_port = htons(config["port"].ToInt());
                serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr_in clientAddress;
    fd_set fileDesciptorReadMaster, fileDesciptorRead;
    timeval timevalue;
            timevalue.tv_sec = 120;
            timevalue.tv_usec = 0;
    WSADATA windowsSocketData;
    const int MAX_QUERY_LENGTH = config["maxQueryLength"].ToInt();

    /* 
        WSADATA is used to store socket information on windows computer.
        WSAStartup is used to initiate socket structure and information
     */
    if(WSAStartup(MAKEWORD(2,2), &windowsSocketData) == SOCKET_ERROR){
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    /*
        Intialiaze the server socket. We will initialise it with  
        1. Address family as internet
        2. Socket type as stream
        3. Protocol as TCP
    */
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket == SOCKET_ERROR){
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    /*
        Bind the socket to local port to reserve the port and utilise it
    */
    if(bind(serverSocket, (sockaddr*)&serverAddress, sizeof(sockaddr)) == SOCKET_ERROR){
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    /*
        Start Listening to the port for any data or connection requet
    */
    if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR){
        WSACleanup();
        exit(EXIT_FAILURE);
    }


    cout<<"Log Server Started successfully..."<<endl<<endl;

    FD_ZERO(&fileDesciptorReadMaster);
    FD_SET(serverSocket, &fileDesciptorReadMaster);

    while(true){
        fileDesciptorRead = fileDesciptorReadMaster;
        int selectResponse = select(serverSocket, &fileDesciptorRead, NULL, NULL, &timevalue);
        for(int i=0; i<selectResponse; i++){
            SOCKET thisSocket = fileDesciptorRead.fd_array[i];
            if(thisSocket == serverSocket){
                /*
                    Accept Socket Connection request and ask for authentication if auth is successfull add to acive client
                */
                int SOCKADDR_LENGTH = sizeof(sockaddr);
                clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &SOCKADDR_LENGTH);

                if(clientSocket == SOCKET_ERROR){
                    /*
                        Failed to accept socket connection
                    */
                    cout<<"Failed to accept connection request"<<endl;
                }else{
                    std::string askForServiceName = "Please provide service name: ";
                    send(clientSocket, askForServiceName.c_str(), askForServiceName.length(), 0);
                    char serviceName[MAX_QUERY_LENGTH] = {0};
                    recv(clientSocket, serviceName, MAX_QUERY_LENGTH, 0);
                    currentClients[clientSocket] = serviceName;
                    CreateLogFiles cfs(serviceName);
                    FD_SET(clientSocket, &fileDesciptorReadMaster);
                    cout<<currentClients[clientSocket]<<" service got Connected to the server."<<endl;
                }

            }else{
                /*
                    A Client is sending some data.
                */
                char logCommand[MAX_QUERY_LENGTH] = {0};
                int bytesIn = recv(thisSocket, logCommand, MAX_QUERY_LENGTH, 0);
                if(bytesIn <= 0){
                    /*  
                        Either the client has closed the connection or some error has occured, 
                        closing client socket connection.
                    */
                    if(bytesIn == SOCKET_ERROR){
                        cout<<"SOCKET ERROR, CODE:"<<WSAGetLastError()<<endl;
                    }else{
                        cout<<currentClients[thisSocket]<<" service was Disconnected."<<endl;
                    }
                    closesocket(thisSocket);
                    FD_CLR(thisSocket, &fileDesciptorReadMaster);
                    FD_CLR(thisSocket, &fileDesciptorRead);
                    currentClients.erase(thisSocket);

                }else if(bytesIn > 2){
                    cout<<"Got :"<<logCommand<<endl;
                    messagingQueue.pushToQueue(currentClients[thisSocket], logCommand);
                }
            }
        }
    }
    return 0;
}