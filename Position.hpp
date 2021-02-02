#ifndef Position_hpp
#define Position_hpp
#include <iostream>

using namespace std;

class Position {
    
public:
    Position();
    Position(int[], int = 1, int = 1);
    void print() const;
    bool operator==(Position&);
    int getWin() const;
    void augmentWin();
    string insertionString() const;
    bool isWinning() const;
    Position fillSquare(int, int);
    bool isEmpty(int) const; // Tells if a square is empty
    bool isDraw() const;
    void addToDisplaySquares(int);
    void setWin(int);
    int getRandDisplaySquare() const;
    void augmentLoss();
    void setLoss(int);
    int getLoss() const;
    double getRatio() const;
    
private:
    int position[3][3];
    int win;
    int loss;
    Position rotate() const; // rotation 90 degrees clockwise.
    Position reflect() const; // reflection over a vertical line down the middle.
    bool equals(int[3][3], int[3][3]) const;
    char toXO(int) const;
    int *displaySquares; // Used when choosing a move for the AI to make.
    int displaySquaresCount;
    
};

#endif
