#include "game.cpp"

int main() {
    GameState<5> game;

    GameState done = game.astar();
    done.printStateHistory();

    return 0;
}


