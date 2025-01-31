#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

struct Pos {
    int x;
    int y;
    Pos(int _x, int _y) {
        x = _x;
        y = _y;
    }
};

template <int size> struct GameState { // make template so game state array can be compile time known
    char squares[2*size*size - 1]; // we store a flat array corresponding to only the valid portions of the board.
    GameState* prevState = nullptr; // if this state was reached by making a move on another board, this points to that previous state
    int wrong =  2*(size*size - 1); // the number of pieces on the wrong side of the board. we win when this reaches 0. (heuristic)
    int holex = size - 1; // x position of the hole
    int holey = size - 1; // y position of the hole
    int movex;
    int movey;

    GameState() {
        int i = 0;
        while (i < size*size - 1) {
            squares[i++] = 'b';
        }
        squares[i++] = ' ';
        while (i < 2*size*size - 1) {
            squares[i++] = 'w';
        }
    }
    bool inbounds(int x, int y) {
        if (x < 0 || y < 0) return false;
        if (x >= size && y < size - 1) return false;
        if (x < size - 1 && y >= size) return false;
        if (x > 2*(size - 1) || y > 2*(size - 1)) return false;
        return true;
    }
    char get(int x, int y) {
        if (inbounds(x, y)) {
            return squares[x*size + y]; // surprisingly simple indexing
        } else {
            return '#'; // invalid space
        }
    }
    void set(int x, int y, char c) {
        squares[y*size + x] = c;
    }
    void print() {
        for (int x = 0; x < 2*size - 1; x++) {
            for (int y = 0; y < 2*size - 1; y++) {
                printf("%c ", get(x, y));
            }
            printf("\n");
        }
        printf("wrong: %d\n", wrong);
    }
    void makeMove(int x, int y) {
        char type = get(x, y);
        movex = x;
        movey = y;
        set(holex, holey, type);
        holex = x;
        holey = y;
        set(holex, holey, ' ');
        if (type == 'w' && x < size && y < size) {
            wrong--;
        }
    }
    GameState<size> clone() const {
        GameState<size> new_state;
        std::copy(std::begin(squares), std::end(squares), std::begin(new_state.squares));
        new_state.wrong = wrong;
        return new_state;
    }
    vector<Pos> getLegalMoves() { // always 8 potential moves: move left, move right, jump left, jump right, etc.
        // Since only one hole ever exists on the board, we can identify moves or actions as just the position of the piece that moves.
        // On any given turn, a peice either has 0 or 1 legal moves, never more.
        vector<Pos> moves; 
        if (get(holex - 1, holey) == 'b') { // if piece to left of hole can move right (is black), add that as a legal move
            moves.push_back(Pos(holex - 1, holey));
        }
        if (get(holex, holey - 1) == 'b') { // if piece above hole can move down, add that as a legal move
            moves.push_back(Pos(holex, holey - 1));
        }
        if (get(holex + 1, holey) == 'w') { // if piece right of hole can move left (is white), add that as a legal move
            moves.push_back(Pos(holex + 1, holey));
        }
        if (get(holex, holey + 1) == 'w') { // if piece below hole can move up, add that as a legal move
            moves.push_back(Pos(holex, holey + 1));
        }
        if (get(holex - 2, holey) == 'b') { // if piece to left of hole can jump right (is black), add that as a legal move
            moves.push_back(Pos(holex - 2, holey));
        }
        if (get(holex, holey - 2) == 'b') { // if piece above hole can jump down, add that as a legal move
            moves.push_back(Pos(holex, holey - 2));
        }
        if (get(holex + 2, holey) == 'w') { // if piece right of hole can jump left (is white), add that as a legal move
            moves.push_back(Pos(holex + 2, holey));
        }
        if (get(holex, holey + 2) == 'w') { // if piece below hole can jump up, add that as a legal move
            moves.push_back(Pos(holex, holey + 2));
        }
        return moves;
    }
    vector<GameState> getSuccessors() {
        vector<GameState> sux;
        for (Pos move : getLegalMoves()) {
            GameState next = clone();
            next.makeMove(move.x, move.y);
            next.prevState = this;
            sux.push_back(next);
        }
        return sux;
    }
    void showLegalMoves() {
        GameState show = clone();
        for (Pos move : getLegalMoves()) {
            printf("move: (%d, %d)\n", move.x, move.y);
            show.set(move.x, move.y, 'x');
        }
        show.print();
    }
    vector<Pos> getMoveHistory() {
        vector<Pos> moves;
        GameState* sptr = this;
        while (sptr != nullptr) {
            moves.push_back(Pos(sptr->movex, sptr->movey));
            sptr = sptr->prevState;
        }
        return moves;
    }
    void printMoveHistory() {
        vector<Pos> moves = getMoveHistory();
        printf("Moves: ");
        for (Pos move: moves) {
            printf(" (%d %d) ", move.x, move.y);
        }
        printf("\n");
    }
    void printStateHistory() {
        vector<GameState> states;
        auto sptr = this;
        while (sptr != nullptr) {
            sptr->print();
            sptr = sptr->prevState;
        }
    }
};






