#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <thread>
#include <vector>
#include<bits/stdc++.h>
#include<string.h>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

enum state { 
    knock, 
    punchline, 
    answer,
    end_of_joke,
};

struct Joke {
    std::string knock;
    std::string response;
    std::string punchline;
    std::string followup;
    std::string answer;
};

class CountingSemaphore {
public:
    CountingSemaphore(int count){

        count_ = count;
        originalCount_ = count;

    }

    // Wait operation
    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ == 0) {
            // If the count is 0, wait until it becomes non-zero
            cv_.wait(lock);
        }
        count_--;
    }

    // Signal operation
    void signal() {
        std::unique_lock<std::mutex> lock(mutex_);
        count_++;
        // Notify one waiting thread, if any
        cv_.notify_one();
    }


    bool active() {

        return count_ == originalCount_ ? false : true;
    }

private:
    int count_;
    int originalCount_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

unsigned long long int totalClientsServed = 0;

CountingSemaphore countingSemaphore(5);

bool parseJson(const std::string& jsonString, std::vector<Joke>& jokes) {
    size_t pos = jsonString.find("\"jokes\":");
    if (pos == std::string::npos) return false;

    pos = jsonString.find("{", pos + 8);
    while (pos != std::string::npos) {
        Joke joke;

        // Find "knock" field and read its value
        pos = jsonString.find("\"knock\":", pos);
        if (pos == std::string::npos) break;
        pos = jsonString.find("\"", pos + 8);
        if (pos == std::string::npos) break;
        size_t startPos = pos + 1;
        pos = jsonString.find("\"", startPos);
        if (pos == std::string::npos) break;
        joke.knock = jsonString.substr(startPos, pos - startPos);

        // Find "response" field and read its value
        pos = jsonString.find("\"response\":", pos);
        if (pos == std::string::npos) break;
        pos = jsonString.find("\"", pos + 12);
        if (pos == std::string::npos) break;
        startPos = pos + 1;
        pos = jsonString.find("\"", startPos);
        if (pos == std::string::npos) break;
        joke.response = jsonString.substr(startPos, pos - startPos);

        // Find "punchline" field and read its value
        pos = jsonString.find("\"punchline\":", pos);
        if (pos == std::string::npos) break;
        pos = jsonString.find("\"", pos + 13);
        if (pos == std::string::npos) break;
        startPos = pos + 1;
        pos = jsonString.find("\"", startPos);
        if (pos == std::string::npos) break;
        joke.punchline = jsonString.substr(startPos, pos - startPos);

        // Find "followup" field and read its value
        pos = jsonString.find("\"followup\":", pos);
        if (pos == std::string::npos) break;
        pos = jsonString.find("\"", pos + 11);
        if (pos == std::string::npos) break;
        startPos = pos + 1;
        pos = jsonString.find("\"", startPos);
        if (pos == std::string::npos) break;
        joke.followup = jsonString.substr(startPos, pos - startPos);

        // Find "answer" field and read its value
        pos = jsonString.find("\"answer\":", pos);
        if (pos == std::string::npos) break;
        pos = jsonString.find("\"", pos + 9);
        if (pos == std::string::npos) break;
        startPos = pos + 1;
        pos = jsonString.find("\"", startPos);
        if (pos == std::string::npos) break;
        joke.answer = jsonString.substr(startPos, pos - startPos);

        jokes.push_back(joke);

        pos = jsonString.find("{", pos);
    }

    return true;
}

void handleClient(int clientSocket) {

    countingSemaphore.wait();
    // Open the JSON file
    std::ifstream ifs("jokes.json");
    if (!ifs.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    // Read the file into a string
    std::string jsonString((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    // Close the file
    ifs.close();

    // Parse the JSON data
    std::vector<Joke> jokes;

    if (!parseJson(jsonString, jokes)) {

        std::cerr << "Failed to parse JSON data." << std::endl;
    }

    std::random_shuffle(jokes.begin(), jokes.end());

    /*
    while(jokes.size() > 3){

        jokes.pop_back();
    }
    */

    int currentJokeIndex = 0;

    state currentState = knock;

    std::string buffer;

    while(currentJokeIndex < jokes.size()){
        
        if(currentState == knock){

            if (write(clientSocket, jokes[currentJokeIndex].knock.c_str(), jokes[currentJokeIndex].knock.size()) == -1) {
                std::cerr << "Error sending message to client." << std::endl;
                break;
            }

            //std::cout<<jokes[currentJokeIndex].knock<<std::endl;

            char bufferData[1024];
            memset(bufferData, 0, sizeof(bufferData));
            int bytesRead = read(clientSocket, bufferData, sizeof(bufferData));
            
            if (bytesRead <= 0) {
                std::cerr << "Error reading from client" << std::endl;
                break;
            } else {
                buffer = bufferData;
                //std::cout << buffer << std::endl;
            }

            if(strcasecmp(buffer.c_str(),jokes[currentJokeIndex].response.c_str()) == 0){

                currentState = punchline;
            
            }else{

                std::string message = "You are supposed to say,\""+jokes[currentJokeIndex].response+"\".Let’s try again.";

                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending message to client." << std::endl;
                    break;
                }

                //std::cout<<message<<std::endl;
                currentState = knock;
                std::this_thread::sleep_for (std::chrono::milliseconds(500));

            }

        }else if(currentState == punchline){

            if (write(clientSocket, jokes[currentJokeIndex].punchline.c_str(), jokes[currentJokeIndex].punchline.size()) == -1) {
                std::cerr << "Error sending message to client." << std::endl;
                break;
            }

            //std::cout<<jokes[currentJokeIndex].punchline<<std::endl;

            char bufferData[1024];
            memset(bufferData, 0, sizeof(bufferData));
            int bytesRead = read(clientSocket, bufferData, sizeof(bufferData));
            
            if (bytesRead <= 0) {
                std::cerr << "Error reading from client." << std::endl;
                break;
            } else {
                buffer = bufferData;
                //std::cout << buffer << std::endl;
            }

            if(strcasecmp(buffer.c_str(),jokes[currentJokeIndex].followup.c_str()) == 0){

                currentState = answer;
            
            }else{

                std::string message = "You are supposed to say,\""+jokes[currentJokeIndex].followup+"\".Let’s try again.";

                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending message to client." << std::endl;
                    break;
                }

                //std::cout<<message<<std::endl;
                currentState = knock;
                std::this_thread::sleep_for (std::chrono::milliseconds(500));

            }

        }else if(currentState == answer){

            if (write(clientSocket, jokes[currentJokeIndex].answer.c_str(), jokes[currentJokeIndex].answer.size()) == -1) {
                std::cerr << "Error sending message to client." << std::endl;
                break;
            }

            //std::cout<<jokes[currentJokeIndex].answer<<std::endl;
            currentState = end_of_joke;

        }else{
            
            std::string message;

            std::this_thread::sleep_for (std::chrono::milliseconds(500));

            if(currentJokeIndex == jokes.size()-1){
                
                message = "I have no more jokes to tell";
                
                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending message to client." << std::endl;
                    break;
                }

                //std::cout << message << std::endl;
                break;

            }else{
                
                message = "Would you like to listen to another?(Y/N)";

                if (write(clientSocket, message.c_str(), message.size()) == -1) {
                    std::cerr << "Error sending message to client." << std::endl;
                    break;
                }

                //std::cout << message << std::endl;

                char bufferData[1024];
                memset(bufferData, 0, sizeof(bufferData));
                int bytesRead = read(clientSocket, bufferData, sizeof(bufferData));
            
                if (bytesRead <= 0) {
                    std::cerr << "Error reading from client." << std::endl;
                    break;
                } else {
                    buffer = bufferData;
                    //std::cout << buffer << std::endl;
                    if(strcasecmp(buffer.c_str(),"Y") == 0){

                        currentJokeIndex++;
                        currentState = knock;
                    
                    }else{

                        break;
                    }
                }


            }
        }
        
    }

    std::cout<<"....Closing Connection....."<<endl;
    // Close the client socket
    close(clientSocket);
    countingSemaphore.signal();
}

int main() {
    
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    //serverAddr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, "10.42.0.121", &serverAddr.sin_addr);

    // Bind socket to address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket to address." << std::endl;
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections." << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    while (true) {
        struct pollfd fds[1];
        fds[0].fd = serverSocket;
        fds[0].events = POLLIN;
        int pollResult = poll(fds, 1, 100);

        if(pollResult == -1){

            std::cerr << "Error in poll." << std::endl;

        }else if(pollResult > 0){

            if(fds[0].revents & POLLIN){

                socklen_t clientAddrSize = sizeof(clientAddr);
                // Accept incoming connection
                clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        
                if (clientSocket == -1) {

                    //Error accepting connection
                    std::cerr << "Error accepting connection." << std::endl;

                }else{

                    // Create a new thread to handle the client
                    std::thread(handleClient, clientSocket).detach();
                    totalClientsServed++;
                }

            }
            
        }else if(pollResult == 0){

            if(totalClientsServed > 0 && !countingSemaphore.active()){

                break;
            }
        }
   
    }

    // Close the server socket
    close(serverSocket);
    return 0;
    

}
