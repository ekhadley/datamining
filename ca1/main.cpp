#include "searches.cpp"

int main() {
    GameState<3> game3;
    // GameState<5> game5;
    // GameState<7> game7;
    // GameState<9> game9;
    // GameState<11> game11;
    // GameState<13> game13;

    auto next1 = game3.getSuccessors()[0];
    auto next2 = next1.getSuccessors()[0];
    auto next3 = next2.getSuccessors()[0];

    next3.printStateHistory();
    
    next3.getMoveHistory();

    return 0;
}
