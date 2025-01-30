#include <iostream>
#include "game.cpp"
#include "searches.cpp"

int main() {
    GameState<3> game3;
    // GameState<5> game5;
    // GameState<7> game7;
    // GameState<9> game9;
    // GameState<11> game11;
    // GameState<13> game13;

    game3.print();
    game3.showLegalMoves();
    game3.makeMove(2, 1);
    game3.showLegalMoves();

    return 0;
}
