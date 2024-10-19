//
// Created by Stefano Candiani on 4/21/24.
//
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;
struct Tile {

    Tile(float x, float y) {
        hidden = true;
        mineTile = false;
        flagTile = false;
        inDebug = false;
        inPause = false;
        xpos = x;
        ypos = y;
        adjMineCount = 0;
    }

    void setPosition(float x, float y) {
        this -> xpos = x;
        this -> ypos = y;
    }

    void reveal() {
        if(hidden && !flagTile) {
            hidden = !hidden;

            if(adjMineCount == 0 && !mineTile) {
                for (int i = 0; i < adjacentTiles.size(); i++) {
                    if (!(adjacentTiles.at(i)->mineTile) && !(adjacentTiles.at(i)->flagTile)) {
                        adjacentTiles.at(i)->reveal();
                    }
                }
            }

        }
    }

    void debug() { inDebug = !inDebug; }

    void pause() { inPause = !inPause; }

    void flag() { if(hidden) { flagTile = !flagTile; } }

    void mine() { if(!mineTile) { mineTile = !mineTile; } }

    bool checkIfPosition(float x, float y) {
        if((x >= xpos && x <= xpos + 32) && (y >= ypos && y <= ypos + 32)){ return true; }
        return false;
    }

    void setUpTile(map<string, sf::Sprite> &sprites) {
        baseSpr = sprites.at("tile_revealed");
        baseSpr.setPosition(xpos,ypos);
        mineSpr = sprites.at("mine");
        mineSpr.setPosition(xpos,ypos);
        topSpr = sprites.at("tile_hidden");
        topSpr.setPosition(xpos, ypos);
        flagSpr = sprites.at("flag");
        flagSpr.setPosition(xpos, ypos);
        mineDebugSpr = sprites.at("mine_debug");
        mineDebugSpr.setPosition(xpos, ypos);
        tilePauseSpr = sprites.at("tile_revealed_pause");
        tilePauseSpr.setPosition(xpos, ypos);
    }

    void drawTile(sf::RenderWindow &window) {
        window.draw(baseSpr);
        if(adjMineCount > 0 && !mineTile) {
            window.draw(numSpr);
        }
        if(mineTile) {
            window.draw(mineSpr);
        }
        if(hidden) {
            window.draw(topSpr);
        }
        if(flagTile) {
            window.draw(flagSpr);
        }
        if(inDebug) {
            window.draw(mineDebugSpr);
        }
        if(inPause) {
            window.draw(tilePauseSpr);
        }
    }

    void addAdjacentTiles(vector<vector<Tile*>> &board, map<string, sf::Sprite> &sprites, int i, int j) {

        /// Anything but edges and corners
        if((i > 0 && j > 0) && (i != board.size() - 1 && j != board.at(i).size() - 1)) {
            adjacentTiles.push_back(board.at(i - 1).at(j - 1));
            adjacentTiles.push_back(board.at(i).at(j - 1));
            adjacentTiles.push_back(board.at(i + 1).at(j - 1));
            adjacentTiles.push_back(board.at(i - 1).at(j));
            adjacentTiles.push_back(board.at(i + 1).at(j));
            adjacentTiles.push_back(board.at(i - 1).at(j + 1));
            adjacentTiles.push_back(board.at(i).at(j + 1));
            adjacentTiles.push_back(board.at(i + 1).at(j + 1));
        }
            /// Top Left Corner
        else if((i == 0 && j == 0)) {
            adjacentTiles.push_back(board.at(i).at(j+1));
            adjacentTiles.push_back(board.at(i+1).at(j+1));
            adjacentTiles.push_back(board.at(i+1).at(j));
        }
            /// Top Right Corner
        else if(i == board.size()-1 && j == 0) {
            adjacentTiles.push_back(board.at(i-1).at(j));
            adjacentTiles.push_back(board.at(i-1).at(j+1));
            adjacentTiles.push_back(board.at(i).at(j+1));
        }
            /// Bottom Left Corner
        else if(i == 0 && j == board.at(i).size()-1) {
            adjacentTiles.push_back(board.at(i).at(j-1));
            adjacentTiles.push_back(board.at(i+1).at(j));
            adjacentTiles.push_back(board.at(i+1).at(j-1));
        }
            /// Bottom Right Corner
        else if(i == board.size()-1 && j == board.at(i).size()-1) {
            adjacentTiles.push_back(board.at(i).at(j-1));
            adjacentTiles.push_back(board.at(i-1).at(j-1));
            adjacentTiles.push_back(board.at(i-1).at(j));
        }
            /// Leftmost Edge
        else if(i == 0 && (j > 0 && j < board.at(i).size()-1)) {
            adjacentTiles.push_back(board.at(i).at(j - 1));
            adjacentTiles.push_back(board.at(i + 1).at(j - 1));
            adjacentTiles.push_back(board.at(i + 1).at(j));
            adjacentTiles.push_back(board.at(i + 1).at(j + 1));
            adjacentTiles.push_back(board.at(i).at(j + 1));
        }
            /// Top edge
        else if(j == 0 && (i > 0 && i < board.size()-1)) {
            adjacentTiles.push_back(board.at(i - 1).at(j));
            adjacentTiles.push_back(board.at(i - 1).at(j + 1));
            adjacentTiles.push_back(board.at(i).at(j + 1));
            adjacentTiles.push_back(board.at(i + 1).at(j + 1));
            adjacentTiles.push_back(board.at(i + 1).at(j));
        }
            /// Rightmost Edge
        else if(i == board.size() - 1 && (j > 0 && j < board.at(i).size()-1)) {
            adjacentTiles.push_back(board.at(i).at(j - 1));
            adjacentTiles.push_back(board.at(i - 1).at(j - 1));
            adjacentTiles.push_back(board.at(i - 1).at(j));
            adjacentTiles.push_back(board.at(i - 1).at(j + 1));
            adjacentTiles.push_back(board.at(i).at(j + 1));
        }
            /// Bottom edge
        else if(j == board.at(i).size() - 1 && (i > 0 && i < board.size()-1)) {
            adjacentTiles.push_back(board.at(i - 1).at(j));
            adjacentTiles.push_back(board.at(i - 1).at(j - 1));
            adjacentTiles.push_back(board.at(i).at(j - 1));
            adjacentTiles.push_back(board.at(i + 1).at(j - 1));
            adjacentTiles.push_back(board.at(i + 1).at(j));
        }

        countAdjacentMines(sprites);

    }

    void countAdjacentMines(map<string, sf::Sprite> &sprites) {
        for(int i = 0; i < adjacentTiles.size(); i++) {
            if(adjacentTiles.at(i) != nullptr && adjacentTiles.at(i) -> mineTile) {
                adjMineCount++;
            }
        }
        if(adjMineCount > 0) {
            string numName = "number_" + to_string(adjMineCount);
            numSpr = sprites.at(numName);
            numSpr.setPosition(xpos,ypos);
        }
    }

    vector<Tile*> adjacentTiles;
    int adjMineCount;
    sf::Sprite baseSpr;
    sf::Sprite numSpr;
    sf::Sprite mineSpr;
    sf::Sprite topSpr;
    sf::Sprite flagSpr;
    sf::Sprite mineDebugSpr;
    sf::Sprite tilePauseSpr;
    bool mineTile;
    bool flagTile;
    bool hidden;
    bool inDebug;
    bool inPause;
    float xpos;
    float ypos;
};
