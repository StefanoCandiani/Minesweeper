#include <iostream>
#include "introcard.cpp"
#include "gamewindow.cpp"
using namespace std;

int main() {

    /// Collect configurations from config.cfg
    ifstream configurations("./files/config.cfg");
    int numCols, numRows, numMines;
    string buffer;
    getline(configurations, buffer);    numCols = stoi(buffer);
    getline(configurations, buffer);    numRows = stoi(buffer);
    getline(configurations, buffer);    numMines = stoi(buffer);
    configurations.close();

    IntroCard welcomeScreen;
    GameWindow letsPlay;
    string name = welcomeScreen(numCols, numRows);
    if(name != "NoProperNameProvided") {
        letsPlay(numCols, numRows, numMines, name);
    }
    return 0;
}

