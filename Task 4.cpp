#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>

using namespace std;

class HttpServer {
public:
    explicit HttpServer(int portNumber) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            cerr << "Failed to create socket" << endl;
            exit(EXIT_FAILURE);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(portNumber);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            cerr << "Binding socket failed" << endl;
            exit(EXIT_FAILURE);
        }
    }

    void begin() {
        if (listen(serverSocket, 5) < 0) {
            cerr << "Listening failed" << endl;
            exit(EXIT_FAILURE);
        }

        cout << "Server started on port 8080..." << endl;

        while (true) {
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int clientSock = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

            if (clientSock < 0) {
                cerr << "Connection acceptance failed" << endl;
                continue;
            }
            thread clientThread(&HttpServer::handleClient, this, clientSock);
            clientThread.detach();
        }
    }

private:
    void handleClient(int clientSock) {
        string request = receiveRequest(clientSock);
        string resource = extractResource(request);
        sendResponse(clientSock, resource);
        close(clientSock);
    }

    string receiveRequest(int clientSock) {
        char buffer[1024];
        stringstream requestStream;

        while (true) {
            int bytes = read(clientSock, buffer, sizeof(buffer));
            if (bytes < 0) {
                cerr << "Failed to read from socket" << endl;
                return "";
            }
            requestStream.write(buffer, bytes);
            if (requestStream.str().find("\r\n\r\n") != string::npos) {
                break;
            }
        }

        return requestStream.str();
    }

    string extractResource(const string& request) {
        size_t methodEnd = request.find(' ');
        if (methodEnd == string::npos || request.substr(0, methodEnd) != "GET") {
            return "/404.html";
        }

        size_t pathStart = methodEnd + 1;
        size_t pathEnd = request.find(' ', pathStart);
        if (pathEnd == string::npos) {
            return "/404.html";
        }

        string resourcePath = request.substr(pathStart, pathEnd - pathStart);
        return resourcePath == "/" ? "/index.html" : resourcePath;
    }

    void sendResponse(int clientSock, const string& resource) {
        string fullPath = "." + resource;
        ifstream resourceFile(fullPath);

        if (!resourceFile) {
            fullPath = "./404.html";
            resourceFile.open(fullPath);
        }

        stringstream responseBody;
        responseBody << resourceFile.rdbuf();

        string response = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: " + to_string(responseBody.str().length()) + "\r\n"
                          "\r\n" + responseBody.str();

        write(clientSock, response.c_str(), response.length());
    }

    int serverSocket;
    sockaddr_in serverAddr;
};

int main() {
    HttpServer server(8080);
    server.begin();
    return 0;
}

