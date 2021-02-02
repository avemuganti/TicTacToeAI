#include "Position.hpp"

Position::Position() {
    
    for (auto& row : position) {
        for (auto& element : row) {
            element = 0;
        }
    }
    
    win = 1;
    loss = 1;
    displaySquares = new int[0];
    displaySquaresCount = 0;
    
}

Position::Position(int array[], int win, int loss) { // array has 9 elements.
    
    int iterator = -1;
    for (auto& row : position) {
        for (auto& element : row) {
            element = array[++iterator];
        }
    }
    
    this -> win = win;
    this -> loss = loss;
    displaySquares = new int[0];
    displaySquaresCount = 0;
    
}

void Position::print() const { // Subject to change once X and O are added.
    
    cout << endl;
    cout << " " << toXO(position[0][0]) << " | " << toXO(position[0][1]) << " | " << toXO(position[0][2]);
    cout << endl;
    cout << "---+---+---" << endl;
    cout << " " << toXO(position[1][0]) << " | " << toXO(position[1][1]) << " | " << toXO(position[1][2]);
    cout << endl;
    cout << "---+---+---" << endl;
    cout << " " << toXO(position[2][0]) << " | " << toXO(position[2][1]) << " | " << toXO(position[2][2]);
    cout << endl << endl;
    
}

Position Position::rotate() const { // Make private later.
    
    int newArray[9];
    newArray[0] = position[2][0];
    newArray[1] = position[1][0];
    newArray[2] = position[0][0];
    newArray[3] = position[2][1];
    newArray[4] = position[1][1];
    newArray[5] = position[0][1];
    newArray[6] = position[2][2];
    newArray[7] = position[1][2];
    newArray[8] = position[0][2];
    Position output(newArray);
    return output;
    
}

Position Position::reflect() const { // Make private later.
    
    int newArray[9];
    newArray[0] = position[0][2];
    newArray[1] = position[0][1];
    newArray[2] = position[0][0];
    newArray[3] = position[1][2];
    newArray[4] = position[1][1];
    newArray[5] = position[1][0];
    newArray[6] = position[2][2];
    newArray[7] = position[2][1];
    newArray[8] = position[2][0];
    Position output(newArray);
    return output;
    
}

bool Position::equals(int arr1[3][3], int arr2[3][3]) const{
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (arr1[i][j] != arr2[i][j]) {
                return false;
            }
        }
    }
    return true;
    
}

bool Position::operator==(Position &other) {
    
    if(equals(position, other.position)) {
        return true;
    }
    if(equals(position, other.rotate().position)) {
        return true;
    }
    if(equals(position, other.rotate().rotate().position)) {
        return true;
    }
    if(equals(position, other.rotate().rotate().rotate().position)) {
        return true;
    }
    if(equals(position, other.reflect().position)) {
        return true;
    }
    if(equals(position, other.reflect().rotate().position)) {
        return true;
    }
    if(equals(position, other.reflect().rotate().rotate().position)) {
        return true;
    }
    if(equals(position, other.reflect().rotate().rotate().rotate().position)) {
        return true;
    }
    return false;
    
}

int Position::getWin() const {
    
    return win;
    
}

void Position::augmentWin() {
    
    ++win;
    
}

char Position::toXO (int num) const {
    
    if (num == 1) {
        return 'X';
    } else if (num == 2) {
        return 'O';
    } else {
        return ' ';
    }
    
}

string Position::insertionString() const {
    
    string output = "";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            output += to_string(position[i][j]);
            output += " ";
        }
    }
    output += to_string(win);
    output += " ";
    output += to_string(loss);
    
    return output;
    
}

bool Position::isWinning() const {
    
    if (position[0][0] == position[0][1] && position [0][1] == position[0][2] && position[0][0] != 0) {
        return true;
    }
    if (position[1][0] == position[1][1] && position [1][1] == position[1][2] && position[1][0] != 0) {
        return true;
    }
    if (position[2][0] == position[2][1] && position [2][1] == position[2][2] && position[2][0] != 0) {
        return true;
    }
    if (position[0][0] == position[1][0] && position [1][0] == position[2][0] && position[0][0] != 0) {
        return true;
    }
    if (position[0][1] == position[1][1] && position [1][1] == position[2][1] && position[0][1] != 0) {
        return true;
    }
    if (position[0][2] == position[1][2] && position [1][2] == position[2][2] && position[0][2] != 0) {
        return true;
    }
    if (position[0][0] == position[1][1] && position [1][1] == position[2][2] && position[0][0] != 0) {
        return true;
    }
    if (position[0][2] == position[1][1] && position [1][1] == position[2][0] && position[0][2] != 0) {
        return true;
    }
    return false;
    
}

Position Position::fillSquare(int square, int value) { // square is a digit 1 - 9.
    
    int array[9];
    int iterator = -1;
    for (auto& row : position) {
        for (auto& element : row) {
            array[++iterator] = element;
        }
    }
    Position output(array, win);
    output.position[(square - 1) / 3][(square - 1) % 3] = value;
    return output;
    
}

bool Position::isEmpty(int square) const {
    
    return position[(square - 1) / 3][(square - 1) % 3] != 1 && position[(square - 1) / 3][(square - 1) % 3] != 2;
    
}

bool Position::isDraw() const {
    
    if (isWinning()) {
        return false;
    }
    for (int i = 0; i < 9; i++) {
        if (isEmpty(i + 1)) {
            return false;
        }
    }
    return true;
    
}

void Position::addToDisplaySquares(int square) {
    
    int *displaySquaresCopy = new int[++displaySquaresCount];
    for (int i = 0; i < displaySquaresCount - 1; i++) {
        displaySquaresCopy[i] = displaySquares[i];
    }
    displaySquaresCopy[displaySquaresCount - 1] = square;
    delete [] displaySquares;
    displaySquares = displaySquaresCopy;
    
}

void Position::setWin(int win) {
    
    this -> win = win;
    
}

int Position::getRandDisplaySquare() const {
    
    srand(static_cast<unsigned>(time(NULL)));
    int random = rand() % displaySquaresCount;
    return displaySquares[random];
    
}

void Position::augmentLoss() {
    
    ++loss;
    
}

int Position::getLoss() const {
    
    return loss;
    
}

void Position::setLoss(int loss) {
    
    this -> loss = loss;
    
}

double Position::getRatio() const {
    
    return (static_cast<double>(win)) / (win + loss);
    
}
