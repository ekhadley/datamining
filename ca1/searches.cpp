#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include "game.cpp"



template <int size> struct BFS { // muast be class to accomodate GameState being a template class
    string name = "DFS";
    vector<Pos> search(GameState<size> startState) {
        vector<Pos> moves;
        auto state = startState;
        while (state.wrong > 0) {

        }


        return moves;
    }
};

