#include "game.cpp"

int main() {
    GameState<3> game3;
    //GameState<5> game5;
    // GameState<7> game7;
    // GameState<9> game9;
    // GameState<11> game11;
    // GameState<13> game13;

    //GameState done = game3.bfs();
    //printf("wrong: %d\n", done.wrong);
    //done.printStateHistory();
    //game3.playGame();


    int size = 3;
    for (int y = 0; y < size; y += 1){
        for (int x = 0; x < size; x += 1) {
            game3.set(x, y, 'w');
        }
    }
    for (int y = size-1; y < size*2-1; y += 1){
        for (int x = size-1; x < size*2-1; x += 1) {
            game3.set(x, y, 'b');
        }
    }
    game3.set(size-1, size-1, ' ');
    game3.set(size - 2, size - 1, 'b');
    game3.set(size , size - 1, 'w');
    game3.wrong = 2;

    printf("very easy starting configuration:\n");
    game3.print();
    printf("wrong = %d\n\n", game3.wrong);

    GameState done = game3.bfs();
    done.printStateHistory();


    return 0;
}


