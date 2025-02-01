#include <iostream>
#include <vector>
#include <math.h>
#include <stack>
#include <queue>
#include <stack>
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

    // Copy constructor
    GameState(const GameState& other) {
        std::copy(std::begin(other.squares), std::end(other.squares), std::begin(squares));
        wrong = other.wrong;
        holex = other.holex;
        holey = other.holey;
        movex = other.movex;
        movey = other.movey;
        prevState = other.prevState;
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
            return squares[y*size + x]; // surprisingly simple indexing
        } else {
            return '#'; // invalid space
        }
    }
    void set(int x, int y, char c) {
        squares[y*size + x] = c;
    }
    void print() {
        for (int y = 0; y < 2*size - 1; y++) {
            for (int x = 0; x < 2*size - 1; x++) {
                printf("%c ", get(x, y));
            }
            printf("\n");
        }
    }
    void makeMove(int x, int y) {
        char type = get(x, y);
        movex = x;
        movey = y;
        set(holex, holey, type);
        if ((type == 'w' && holex < size && holey < size) || (type == 'b' && holex >= size && holey >= size)) {
            wrong--;
        }
        holex = x;
        holey = y;
        set(holex, holey, ' ');
    }
    GameState<size> clone() const {
        GameState<size> new_state;
        std::copy(std::begin(squares), std::end(squares), std::begin(new_state.squares));
        new_state.wrong = wrong;
        new_state.holex = holex;
        new_state.holey = holey;
        new_state.movex = movex;
        new_state.movey = movey;
        new_state.prevState = prevState;
        return new_state;
    }
    vector<Pos> getLegalMoves() { // always 8 potential moves: move left, move right, jump left, jump right, etc.
        // Since only one hole ever exists on the board, we can identify moves or actions as just the position of the piece that moves.
        // On any given turn, a peice either has 0 or 1 legal moves, never more.
        vector<Pos> moves; 
        if (get(holex - 1, holey) == 'b') { // if piece to left of hole can move right (is black), add that as a legal move
            /*if (holex == 1 && holey == 2) { printf("1 passed\n"); }*/
            moves.push_back(Pos(holex - 1, holey));
        }
        if (get(holex, holey - 1) == 'b') { // if piece above hole can move down, add that as a legal move
            /*if (holex == 1 && holey == 2) { printf("2 passed\n"); }*/
            moves.push_back(Pos(holex, holey - 1));
        }
        if (get(holex - 2, holey) == 'b') { // if piece to left of hole can jump right (is black), add that as a legal move
            /*if (holex == 1 && holey == 2) { printf("3 passed\n"); }*/
            moves.push_back(Pos(holex - 2, holey));
        }
        if (get(holex, holey - 2) == 'b') { // if piece above hole can jump down, add that as a legal move
            /*if (holex == 1 && holey == 2) { printf("4 passed\n"); }*/
            moves.push_back(Pos(holex, holey - 2));
        }
        if (get(holex + 1, holey) == 'w') { // if piece right of hole can move left (is white), add that as a legal move
            /*if (holex == 1 && holey == 2) { printf("5 passed\n"); }*/
            moves.push_back(Pos(holex + 1, holey));
        }
        if (get(holex, holey + 1) == 'w') { // if piece below hole can move up, add that as a legal move
            /*if (holex == 1 && holey == 2) { printf("6 passed\n"); }*/
            moves.push_back(Pos(holex, holey + 1));
        }
        if (get(holex + 2, holey) == 'w') { // if piece right of hole can jump left (is white), add that as a legal move
            /*if (holex == 1 && holey == 2) { printf("7 passed\n"); }*/
            moves.push_back(Pos(holex + 2, holey));
        }
        if (get(holex, holey + 2) == 'w') { // if piece below hole can jump up, add that as a legal move
            /*if (holex == 1 && holey == 2) { printf("8 passed\n"); }*/
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
        std::stack<GameState*> stateStack;
        auto sptr = this;
        while (sptr != nullptr) {
            stateStack.push(sptr);
            sptr = sptr->prevState;
        }
        while (!stateStack.empty()) {
            stateStack.top()->print();
            stateStack.pop();
        }
    }
    GameState<size> BFS() {
        queue<GameState> front;
        front.push(*this);
        while (!front.empty()) {
            GameState state = front.front();
            front.pop();
            printf("current wrong: %d, nstates: %d\n", state.wrong, front.size());
            if (state.wrong == 0) return state;
            for (auto succ : state.getSuccessors()) {
                front.push(succ);
            }
        }
        return *this;
    }
};






