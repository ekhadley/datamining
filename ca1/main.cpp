#include "searches.cpp"

int main() {
    GameState<3> game3;
    // GameState<5> game5;
    // GameState<7> game7;
    // GameState<9> game9;
    // GameState<11> game11;
    // GameState<13> game13;

    game3.print();
    game3.makeMove(1, 2);
    game3.print();
    game3.makeMove(3, 2);
    game3.print();

    return 0;
}
