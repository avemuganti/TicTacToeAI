#include <iostream>
#include <fstream>
#include "Position.hpp"
#include <math.h>

using namespace std;

bool PLAY_TO_WIN = false;
int NUM_ROUNDS = 100;

int numKnown;
int playedSize = 0;
Position *knownPositions; // Array of known positions.
Position *playedPositions = new Position[0]; // Array of positions played by AI this game.
Position *allPositions = new Position[0]; // All possible moves for a particular turn, one of which will be added to playedPositions and the others of which will be added to otherPositions.

int numKnown2;
int playedSize2 = 0;
Position *knownPositions2; // Array of known positions.
Position *playedPositions2 = new Position[0]; // Array of positions played by AI this game.
Position *allPositions2 = new Position[0]; // All possible moves for a particular turn, one of which will be added to playedPositions and the others of which will be added to otherPositions.

Position currentPosition; // Keeps track of player and AI positions. To be displayed

void readFromFile(string, int&, Position*&);
void readToFile(string, int, Position*);
bool playAiXTurn();
bool playAiOTurn();
bool playPlayerXTurn();
bool playPlayerOTurn();

int main() {
    
    string xFileName = "AISaveFile.txt";
    string oFileName = "AISaveFile2.txt";
    
//------------------------------------------------------------------------------------------
    
    for (int i = 0; i < NUM_ROUNDS; i++) {
    
        readFromFile(xFileName, numKnown, knownPositions); // Varies with players.
        readFromFile(oFileName, numKnown2, knownPositions2); // Varies with players.
    
        currentPosition.print();
    
        while(true) {
    
            if (playAiXTurn()) {
                break;
            }
        
            if (playAiOTurn()) {
                break;
            }
        
        } // end while
    
        readToFile(xFileName, numKnown, knownPositions); // Varies with players.
        readToFile(oFileName, numKnown2, knownPositions2); // Varies with players.
        
        delete [] knownPositions;
        delete [] knownPositions2;
        playedSize = 0;
        playedSize2 = 0;
        numKnown = 0;
        numKnown2 = 0;
        delete [] playedPositions;
        delete [] allPositions;
        delete [] playedPositions2;
        delete [] allPositions2;
        playedPositions = new Position[0];
        allPositions = new Position[0];
        playedPositions2 = new Position[0];
        allPositions2 = new Position[0];
        Position p;
        currentPosition = p;
        
    } // end for

//------------------------------------------------------------------------------------------

    return 0;
    
}

void readFromFile(string fileName, int &numKnown, Position* &knownPositions) {
    
    ifstream input;
   
    input.open(fileName);
    input >> numKnown;
    
    knownPositions = new Position[numKnown];
    
    for (int i = 0; i < numKnown; i++) {
        int array[9];
        int win;
        int loss;
        for (int j = 0; j < 9; j++) {
            input >> array[j];
        }
        input >> win;
        input >> loss;
        knownPositions[i] = Position(array, win, loss);
    }
    
    input.close();
    
}

void readToFile(string fileName, int numKnown, Position *knownPositions) {
    
    ofstream output;
    
    output.open(fileName);
    
    output << numKnown << endl;
    for (int i = 0; i < numKnown; i++) {
        output << knownPositions[i].insertionString() << endl;
    }
    
    output.close();
    
}

bool playAiXTurn() {
    
    srand(static_cast<unsigned>(time(NULL)));
    
    int allPositionsCounter = 0;
    for (int i = 1; i <= 9; i++) {
        if (currentPosition.isEmpty(i)) {
            bool flag = false;
            for (int j = 0; j < allPositionsCounter; j++) {
                if (currentPosition.fillSquare(i, 1) == allPositions[j]) {
                    allPositions[j].addToDisplaySquares(i);
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                Position *temp = new Position[++allPositionsCounter];
                for (int j = 0; j < allPositionsCounter - 1; j++) {
                    temp[j] = allPositions[j];
                }
                temp[allPositionsCounter - 1] = currentPosition.fillSquare(i, 1);
                delete [] allPositions;
                allPositions = temp;
                allPositions[allPositionsCounter - 1].addToDisplaySquares(i);
            }
        }
    }
    
    for (int i = 0; i < allPositionsCounter; i++) {
        bool flag = false; // If allPositions[i] is a known position, this changes to true.
        for (int j = 0; j < numKnown; j++) {
            if (allPositions[i] == knownPositions[j]) {
                flag = true;
                allPositions[i].setWin(knownPositions[j].getWin());
                allPositions[i].setLoss(knownPositions[j].getLoss());
                break;
            }
        }
        if (!flag) {
            Position *temp = new Position[++numKnown];
            for (int j = 0; j < numKnown - 1; j++) {
                temp[j] = knownPositions[j];
            }
            temp[numKnown - 1] = allPositions[i];
            delete [] knownPositions;
            knownPositions = temp;
        }
    }
    
    Position winner;
    if (!PLAY_TO_WIN) {
        double winSum = 0;
        for (int i = 0; i < allPositionsCounter; i++) {
            winSum += allPositions[i].getRatio();
        }
        double randValue = fmod(rand(), winSum);
        double cumulativeSum = 0;
        for (int i = 0; i < allPositionsCounter; i++) {
            cumulativeSum += allPositions[i].getRatio();
            if (randValue < cumulativeSum) {
                winner = allPositions[i];
                break;
            }
        }
    } else {
        double greatestWin = 0;
        for (int i = 0; i < allPositionsCounter; i++) {
            if (allPositions[i].getRatio() > greatestWin) {
                greatestWin = allPositions[i].getRatio();
                winner = allPositions[i];
            }
        }
    }
    
    Position *playedTemp = new Position[++playedSize];
    for (int i = 0; i < playedSize - 1; i++) {
        playedTemp[i] = playedPositions[i];
    }
    playedTemp[playedSize - 1] = winner;
    delete [] playedPositions;
    playedPositions = playedTemp;
    
    currentPosition = winner;
    
    Position *allTemp = new Position[0];
    delete [] allPositions;
    allPositions = allTemp;
    
    currentPosition.print();
    
    if (currentPosition.isWinning()) {
        cout << "Game End! AI X Wins!" << endl;
        for (int i = 0; i < playedSize; i++) {
            for (int j = 0; j < numKnown; j++) {
                if (playedPositions[i] == knownPositions[j]) {
                    knownPositions[j].augmentWin();
                    break;
                }
            }
        }
        for (int i = 0; i < playedSize2; i++) {
            for (int j = 0; j < numKnown2; j++) {
                if (playedPositions2[i] == knownPositions2[j]) {
                    knownPositions2[j].augmentLoss();
                    break;
                }
            }
        }
        return true;
    }
    
    if (currentPosition.isDraw()) {
        cout << "Game End! It's a Draw!" << endl;
        return true;
    }
    
    return false;
    
}

bool playAiOTurn() {
    
    srand(static_cast<unsigned>(time(NULL)));
    
    int allPositionsCounter = 0;
    for (int i = 1; i <= 9; i++) {
        if (currentPosition.isEmpty(i)) {
            bool flag = false;
            for (int j = 0; j < allPositionsCounter; j++) {
                if (currentPosition.fillSquare(i, 2) == allPositions2[j]) {
                    allPositions2[j].addToDisplaySquares(i);
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                Position *temp = new Position[++allPositionsCounter];
                for (int j = 0; j < allPositionsCounter - 1; j++) {
                    temp[j] = allPositions2[j];
                }
                temp[allPositionsCounter - 1] = currentPosition.fillSquare(i, 2);
                delete [] allPositions2;
                allPositions2 = temp;
                allPositions2[allPositionsCounter - 1].addToDisplaySquares(i);
            }
        }
    }
    
    for (int i = 0; i < allPositionsCounter; i++) {
        bool flag = false; // If allPositions[i] is a known position, this changes to true.
        for (int j = 0; j < numKnown2; j++) {
            if (allPositions2[i] == knownPositions2[j]) {
                flag = true;
                allPositions2[i].setWin(knownPositions2[j].getWin());
                allPositions2[i].setLoss(knownPositions2[j].getLoss());
                break;
            }
        }
        if (!flag) {
            Position *temp = new Position[++numKnown2];
            for (int j = 0; j < numKnown2 - 1; j++) {
                temp[j] = knownPositions2[j];
            }
            temp[numKnown2 - 1] = allPositions2[i];
            delete [] knownPositions2;
            knownPositions2 = temp;
        }
    }
    
    Position winner;
    if (!PLAY_TO_WIN) {
        double winSum = 0;
        for (int i = 0; i < allPositionsCounter; i++) {
            winSum += allPositions2[i].getRatio();
        }
        double randValue = fmod(rand(), winSum);
        double cumulativeSum = 0;
        for (int i = 0; i < allPositionsCounter; i++) {
            cumulativeSum += allPositions2[i].getRatio();
            if (randValue < cumulativeSum) {
                winner = allPositions2[i];
                break;
            }
        }
    } else {
        double greatestWin = 0;
        for (int i = 0; i < allPositionsCounter; i++) {
            if (allPositions2[i].getRatio() > greatestWin) {
                greatestWin = allPositions2[i].getRatio();
                winner = allPositions2[i];
            }
        }
    }
    
    Position *playedTemp = new Position[++playedSize2];
    for (int i = 0; i < playedSize2 - 1; i++) {
        playedTemp[i] = playedPositions2[i];
    }
    playedTemp[playedSize2 - 1] = winner;
    delete [] playedPositions2;
    playedPositions2 = playedTemp;
    
    currentPosition = winner;
    
    Position *allTemp = new Position[0];
    delete [] allPositions2;
    allPositions2 = allTemp;
    
    currentPosition.print();
    
    if (currentPosition.isWinning()) {
        cout << "Game End! AI O Wins!" << endl;
        for (int i = 0; i < playedSize2; i++) {
            for (int j = 0; j < numKnown2; j++) {
                if (playedPositions2[i] == knownPositions2[j]) {
                    knownPositions2[j].augmentWin();
                    break;
                }
            }
        }
        for (int i = 0; i < playedSize; i++) {
            for (int j = 0; j < numKnown; j++) {
                if (playedPositions[i] == knownPositions[j]) {
                    knownPositions[j].augmentLoss();
                    break;
                }
            }
        }
        return true;
    }
    
    if (currentPosition.isDraw()) {
        cout << "Game End! It's a Draw!" << endl;
        return true;
    }
    
    return false;
    
}

bool playPlayerXTurn() {
    
    int playerSquare;
        
    do {
        cout << "What square would you like to play on?: ";
        cin >> playerSquare;
    } while (playerSquare < 1 || playerSquare > 9 || !currentPosition.isEmpty(playerSquare));
        
    currentPosition = currentPosition.fillSquare(playerSquare, 1);
        
    currentPosition.print();
        
    if (currentPosition.isWinning()) {
        cout << "Game End! Player Wins!" << endl;
        for (int i = 0; i < playedSize2; i++) {
            for (int j = 0; j < numKnown2; j++) {
                if (playedPositions2[i] == knownPositions2[j]) {
                    knownPositions2[j].augmentLoss();
                    break;
                }
            }
        }
        return true;
    }
    
    if (currentPosition.isDraw()) {
        cout << "Game End! It's a Draw!" << endl;
        return true;
    }
    
    return false;
    
}

bool playPlayerOTurn() {
    
    int playerSquare;
        
    do {
        cout << "What square would you like to play on?: ";
        cin >> playerSquare;
    } while (playerSquare < 1 || playerSquare > 9 || !currentPosition.isEmpty(playerSquare));
        
    currentPosition = currentPosition.fillSquare(playerSquare, 2);
        
    currentPosition.print();
        
    if (currentPosition.isWinning()) {
        cout << "Game End! Player Wins!" << endl;
        for (int i = 0; i < playedSize; i++) {
            for (int j = 0; j < numKnown; j++) {
                if (playedPositions[i] == knownPositions[j]) {
                    knownPositions[j].augmentLoss();
                    break;
                }
            }
        }
        return true;
    }
    
    if (currentPosition.isDraw()) {
        cout << "Game End! It's a Draw!" << endl;
        return true;
    }
    
    return false;
    
}
