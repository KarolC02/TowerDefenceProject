#include "Game.hpp"
#include <iostream>
#include <memory>

int main() {
    int port = 8081;
    std::unique_ptr<Game> game = std::make_unique<Game>(port);
    game->Run();
    return EXIT_SUCCESS;
}
