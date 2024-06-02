#pragma once

#include <thread>
#include <atomic>
#include <netinet/in.h>

class Game; // Forward declaration

class SocketServer {
public:
    SocketServer(int port, Game& game);
    ~SocketServer();
    void start();
    void stop();
    bool isRunning() const;

private:
    int port;
    std::atomic<bool> running;
    int serverSocket;
    Game& game;
    std::thread serverThread;

    void acceptConnections();
    void handleClient(int clientSocket);
    void log(const std::string& message);
};
