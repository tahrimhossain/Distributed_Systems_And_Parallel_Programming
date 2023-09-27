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
#include <functional>
using namespace std;

enum state { 
    knock, 
    punchline, 
    answer,
    end_of_joke
};


/*
Example:
Server: Knock, knock!
Client: Who that?
Server: You are supposed to say,"Who's there?".Let’s try again.
*/
void testWrongResponse(int clientSocket){

    std::string message, buffer;

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

            if(buffer.find("Let’s try again") != std::string::npos){

                currentState = knock;

            }else{

                currentState = knock;

                message = "Who that?";
                
                std::cout << "Client: ";
                
                
                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending data to server." << std::endl;
                    return;
                }

                cout << message << endl;
                currentState = punchline;
            }

            

        }else if(currentState == punchline){

            if(buffer.find("Let’s try again") != std::string::npos){
                std::cout<<"Test case passed"<<std::endl;
                currentState = knock;
                return;
                

            }else{

                std::cout<<"Test case failed"<<std::endl;
                return;
            }
            

        }else{

            std::cout<<"Test case failed"<<std::endl;
            return;
        } 

    }

}

/*
Example:
Server: Knock, knock!
Client: Who's there?
Server: Robin
Client: who?
Server: You are supposed to say,"Robin who?".Let’s try again.
*/
void testWrongFollowup(int clientSocket){

    std::string message, buffer;

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

            if(buffer.find("Let’s try again") != std::string::npos){

                currentState = knock;

            }else{

                currentState = knock;

                message = "Who's there?";
                
                std::cout << "Client: ";
                
                
                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending data to server." << std::endl;
                    return;
                }

                cout << message << endl;
                currentState = punchline;
            }

            

        }else if(currentState == punchline){

            if(buffer.find("Let’s try again") != std::string::npos){

                std::cout<< "Test case failed" <<std::endl;
                currentState = knock;
                return;

            }else{

                message = "who?";
                
                std::cout << "Client: ";
                
                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending data to server." << std::endl;
                    return;
                }

                cout << message << endl;
                currentState = answer;
            }
            

        }else if(currentState == answer){

            if(buffer.find("Let’s try again") != std::string::npos){

                std::cout<< "Test case passed" <<std::endl;
                currentState = knock;
                return;
            
            }else{

                std::cout<< "Test case failed" <<std::endl;
                currentState = end_of_joke;
                return;
            }

        }else{

            std::cout<< "Test case failed" <<std::endl;
            return;

        } 

    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <function_name>\n";
        return 1;
    }

    std::string functionName = argv[1];

    if (functionName != "testWrongResponse" && functionName != "testWrongFollowup") {
        
        std::cout << "Invalid function name!\n";
        return -1;
    }


    int clientSocket;
    struct sockaddr_in serverAddr;
    

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

    
    if(functionName == "testWrongResponse"){

        testWrongResponse(clientSocket);

    }else{

        testWrongFollowup(clientSocket);
    }

    // Close the client socket
    close(clientSocket);
    return 0;
}
