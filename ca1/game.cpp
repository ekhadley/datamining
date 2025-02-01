#include <iostream>
#include <vector>
#include <math.h>
#include <stack>
#include <queue>
#include <stack>
#include <unordered_set>
#include <memory> // Add this include for smart pointers
#include <set>
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
    shared_ptr<GameState> prevState = nullptr; // Use shared_ptr instead of raw pointer
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
        copy(begin(other.squares), end(other.squares), begin(squares));
        wrong = other.wrong;
        holex = other.holex;
        holey = other.holey;
        movex = other.movex;
        movey = other.movey;
        prevState = other.prevState; // shared_ptr handles copying
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
        // if x + y == 2*(size - 1), then (x, y) is on the diagonal
        // if x + y > 2*(size - 1)
        int ldist = 2*(size - 1);
        if (type == 'b' && (x + y <= ldist) && (holex + holey > ldist)) {
            wrong--;
        }
        if (type == 'w' && (x + y >= ldist) && (holex + holey < ldist)) {
            wrong--;
        }
        holex = x;
        holey = y;
        set(holex, holey, ' ');
    }
    GameState<size> clone() const {
        GameState<size> new_state;
        copy(begin(squares), end(squares), begin(new_state.squares));
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
        // On any given turn, a peice either has 0 or 1 legal moves, never more, so we can represent moves as just a simple coordinate.
        vector<Pos> moves; 
        if (get(holex - 1, holey) == 'b') { // if piece to left of hole can move right (is black), add that as a legal move
            moves.push_back(Pos(holex - 1, holey));
        }
        if (get(holex, holey - 1) == 'b') { // if piece above hole can move down, add that as a legal move
            moves.push_back(Pos(holex, holey - 1));
        }
        if (get(holex - 2, holey) == 'b') { // if piece to left of hole can jump right (is black), add that as a legal move
            moves.push_back(Pos(holex - 2, holey));
        }
        if (get(holex, holey - 2) == 'b') { // if piece above hole can jump down, add that as a legal move
            moves.push_back(Pos(holex, holey - 2));
        }
        if (get(holex + 1, holey) == 'w') { // if piece right of hole can move left (is white), add that as a legal move
            moves.push_back(Pos(holex + 1, holey));
        }
        if (get(holex, holey + 1) == 'w') { // if piece below hole can move up, add that as a legal move
            moves.push_back(Pos(holex, holey + 1));
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
            next.prevState = make_shared<GameState>(*this); // shared_ptr so we can print after returning from the search
            sux.push_back(next);
        }
        return sux;
    }
    void printLegalMoves() {
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
            sptr = sptr->prevState.get();
        }
        return moves;
    }
    void printMoveHistory() {
        if (this->prevState != nullptr) {
            this->prevState->printMoveHistory();
        }
        printf(" (%d %d) ", movex, movey);
    }

    void printStateHistory() {
        if (prevState != nullptr) {
            this->prevState.get()->printStateHistory();
        }
        printf("\n");
        print();
    }
    bool operator==(const GameState& other) const {
        if (this->wrong != other.wrong) return false; // easy checks first
        if (this->holex != other.holex) return false;
        if (this->holey != other.holey) return false;
        for (int i = 0; i < 2*size*size - 1; i++) { // otherwise check piece colors
            if (this->squares[i] != other.squares[i]) return false;
        }
        return true;
    }
    string toString() const {
        return string(squares, squares + sizeof(squares));
    }
    

    bool isLessWrong(const GameState<size>& a, const GameState<size>& b) {
        return b.wrong < a.wrong;
    }

    GameState<size> astar() {
        priority_queue<GameState<size>, GameState<size>::isLessWrong> queue;
        unordered_set<string> visited;
        queue.push(*this);
        visited.insert(this->toString());
        while (!queue.empty()) {
            GameState<size> current = queue.top();
            queue.pop();
            if (queue.size()%1000 == 0) printf("size: %d, wrong: %d\n", queue.size(), current.wrong);
            if (current.wrong == 0) return current;
            for (GameState<size> successor : current.getSuccessors()) {
                string stateStr = successor.toString();
                if (visited.find(stateStr) == visited.end()) {
                    visited.insert(stateStr);
                    queue.push(successor);
                }
            }
        }
        printf("No solution found by astar.\n");
        return *this;
    }

};
