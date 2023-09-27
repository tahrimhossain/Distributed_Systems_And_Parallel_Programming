#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<string.h>
#include<bits/stdc++.h>
#include<thread>
#include<chrono>
using namespace std;

enum state { 
    knock, 
    punchline, 
    answer,
    end_of_joke
};

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    std::string message, buffer;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    //inet_pton(AF_INET, "192.168.0.159", &serverAddr.sin_addr);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server." << std::endl;
        close(clientSocket);
        return -1;
    }

    state currentState = knock;

    while(1){
        
        char bufferData[1024];
        memset(bufferData, 0, sizeof(bufferData));
        int bytesRead = read(clientSocket, bufferData, sizeof(bufferData));
        if (bytesRead <= 0) {
            std::cerr << "Error reading from server." << std::endl;
        } else {
            buffer = bufferData;
            std::cout<<"Server: ";
            std::cout << buffer << std::endl;
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(500));

        if(currentState == knock){

            if(buffer.find("Let’s try again") != std::string::npos || buffer.find("You are supposed to say") != std::string::npos){

                currentState = knock;

            }else{

                currentState = knock;

                //message = "Who's there?";
                
                std::cout << "Client: ";
                getline(cin,message);
                
                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending data to server." << std::endl;
                    close(clientSocket);
                    return -1;
                }

                //cout << message << endl;
                currentState = punchline;
            }

            

        }else if(currentState == punchline){

            if(buffer.find("Let’s try again") != std::string::npos || buffer.find("You are supposed to say") != std::string::npos){

                currentState = knock;

            }else{

                //message = buffer + " who?";
                
                std::cout << "Client: ";
                getline(cin,message);

                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending data to server." << std::endl;
                    close(clientSocket);
                    return -1;
                }

                //cout << message << endl;
                currentState = answer;
            }
            

        }else if(currentState == answer){

            if(buffer.find("Let’s try again") != std::string::npos || buffer.find("You are supposed to say") != std::string::npos){

                currentState = knock;
            
            }else{

                currentState = end_of_joke;
            }

        }else{

            std::string input;
            
            if(buffer.find("Would you like to listen to another?(Y/N)") != std::string::npos){

                while(1){

                    std::cout << "Client: ";
                    getline(cin,input);

                    if(input == "Y" || input == "y"){
                        currentState = knock;
                    
                        if (write(clientSocket, "Y", input.size()) == -1) {
                            std::cerr << "Error sending data to server." << std::endl;
                            close(clientSocket);
                            return -1;
                        }

                        break;

                    }else if(input == "N" || input == "n"){

                        if (write(clientSocket, "N", input.size()) == -1) {
                            std::cerr << "Error sending data to server." << std::endl;
                            close(clientSocket);
                            return -1;
                        }

                        break;
                        
                    }else{

                        std::cout << "Server: ";
                        std::cout << buffer << std::endl;
                    }

                }

                if(input == "N" || input == "n"){

                    break;
                }
                
                
            }else{

                break;
            }

        } 

    }

    // Close the client socket
    close(clientSocket);
    return 0;
}
