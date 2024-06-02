#include "SocketServer.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include "Game.hpp"

SocketServer::SocketServer(int port, Game& gameInstance)
    : port(port), running(false), serverSocket(-1), game(gameInstance) {}

SocketServer::~SocketServer() {
    stop();
}

void SocketServer::start() {
    running = true;
    serverThread = std::thread(&SocketServer::acceptConnections, this);
}

void SocketServer::stop() {
    if (running) {
        running = false;
        close(serverSocket);
        if (serverThread.joinable()) {
            serverThread.join();
        }
    }
}

bool SocketServer::isRunning() const {
    return running;
}

void SocketServer::acceptConnections() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        log("Failed to create socket");
        return;
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        log("Failed to bind socket");
        return;
    }

    if (listen(serverSocket, 5) < 0) {
        log("Failed to listen on socket");
        return;
    }

    log("Server started on port " + std::to_string(port));

    while (running) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket >= 0) {
            log("Client connected");
            handleClient(clientSocket);
        }
    }
}

void SocketServer::handleClient(int clientSocket) {
    std::thread([this, clientSocket]() {
        char buffer[1024];
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::string command(buffer);

            log("Received command: " + command);

            if (command.find("placeTower") == 0) {
                int towerType, gridX, gridY;
                sscanf(command.c_str(), "placeTower %d %d %d", &towerType, &gridX, &gridY);
                game.placeTowerAtGrid(static_cast<TowerType>(towerType), gridX, gridY);
            } else if (command.find("sellTower") == 0) {
                int gridX, gridY;
                sscanf(command.c_str(), "sellTower %d %d", &gridX, &gridY);
                game.sellTowerAtGrid(gridX, gridY);
            } else if (command.find("upgradeTower") == 0) {
                int gridX, gridY;
                sscanf(command.c_str(), "upgradeTower %d %d", &gridX, &gridY);
                game.upgradeTowerAtGrid(gridX, gridY);
            } else if (command.find("startGame") == 0) {
                game.startGame();
            } else if (command.find("resetGame") == 0) {
                game.resetGame();
            }

            std::string response = "OK\n";
            write(clientSocket, response.c_str(), response.size());
        }
        close(clientSocket);
    }).detach();
}

void SocketServer::log(const std::string& message) {
    std::cout << message << std::endl;
}
