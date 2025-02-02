#include "game.cpp"
// group members: Ethan Hadley.
// Search method is greedy, using only the number of pieces on the wrong side of the board as the heuristic.
// The greedy search only works for size 7 (which is GameState<3> here. 3 is the size of each square of colored pieces)
// Making it A* instead of greedy requires only 1 line change (see Compararer). A* doesnt even work for size 7, let alone size 11.
// Neither did my DFS, greedy was the only one that could solve 7, none of the searches could even solve size 9.
// I have no idea why my searches are doing so bad. I'm pretty sure my game rules are right. It's not that they are just slow,
// the searches just continue forever until I run out of ram. I would guess my searches all share some common bug, and not that
// the game is implemented wrong, but I don't know. Greedy search on 5 and 7 both run about instantly for me, well under 10 seconds.

// compiled with 'g++ main.cpp'
// output files are called 'greedyX.out' where X is board size. an example output is included.

int main() {
    int puzzleSize;
    std::cout << "Enter puzzle size (5 or 7): ";
    std::cin >> puzzleSize;

    if (puzzleSize == 5) {
        GameState<3> game;
        GameState done = game.search();
        done.saveStateHistory();
        return 0;
        //done.printStateHistory();
    } else if (puzzleSize == 7) {
        GameState<4> game;
        GameState done = game.search();
        done.saveStateHistory();
        return 0;
        //done.printStateHistory();
    } else {
        std::cout << "Invalid puzzle size. Search only works for size 5 or 7." << std::endl;
    }

    return -1;
}


