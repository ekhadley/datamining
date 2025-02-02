#include "game.cpp"

int main() {
    GameState<4> game;

    GameState done = game.search();
    //done.printStateHistory();

    return 0;
}


