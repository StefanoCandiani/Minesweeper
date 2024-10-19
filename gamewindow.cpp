//
// Created by Stefano Candiani on 4/10/24.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "leaderboard.cpp"
#include "tile.cpp"
using namespace std;

struct GameWindow {

    void operator()(int numCols, int numRows, int numMines, string name) {
        bool quit = false;
        Leaderboard LB;

        /// Storing all sprites
        sf::Texture texture1; texture1.loadFromFile("./files/images/debug.png");
        sprites.emplace("debug", sf::Sprite(texture1));
        sf::Texture texture2;
        texture2.loadFromFile("./files/images/digits.png");
        sprites.emplace("digits", sf::Sprite(texture2));
        sf::Texture texture3;
        texture3.loadFromFile("./files/images/face_happy.png");
        sprites.emplace("face_happy", sf::Sprite(texture3));
        sf::Texture texture4;
        texture4.loadFromFile("./files/images/face_lose.png");
        sprites.emplace("face_lose", sf::Sprite(texture4));
        sf::Texture texture5;
        texture5.loadFromFile("./files/images/face_win.png");
        sprites.emplace("face_win", sf::Sprite(texture5));
        sf::Texture texture6;
        texture6.loadFromFile("./files/images/flag.png");
        sprites.emplace("flag", sf::Sprite(texture6));
        sf::Texture texture7;
        texture7.loadFromFile("./files/images/leaderboard.png");
        sprites.emplace("leaderboard", sf::Sprite(texture7));
        sf::Texture texture8;
        texture8.loadFromFile("./files/images/mine.png");
        sprites.emplace("mine", sf::Sprite(texture8));
        sprites.emplace("mine_debug",sf::Sprite(texture8));
        sf::Texture texture9_1;
        texture9_1.loadFromFile("./files/images/number_1.png");
        sprites.emplace("number_1", sf::Sprite(texture9_1));
        sf::Texture texture9_2;
        texture9_2.loadFromFile("./files/images/number_2.png");
        sprites.emplace("number_2", sf::Sprite(texture9_2));
        sf::Texture texture9_3;
        texture9_3.loadFromFile("./files/images/number_3.png");
        sprites.emplace("number_3", sf::Sprite(texture9_3));
        sf::Texture texture9_4;
        texture9_4.loadFromFile("./files/images/number_4.png");
        sprites.emplace("number_4", sf::Sprite(texture9_4));
        sf::Texture texture9_5;
        texture9_5.loadFromFile("./files/images/number_5.png");
        sprites.emplace("number_5", sf::Sprite(texture9_5));
        sf::Texture texture9_6;
        texture9_6.loadFromFile("./files/images/number_6.png");
        sprites.emplace("number_6", sf::Sprite(texture9_6));
        sf::Texture texture9_7;
        texture9_7.loadFromFile("./files/images/number_7.png");
        sprites.emplace("number_7", sf::Sprite(texture9_7));
        sf::Texture texture9_8;
        texture9_8.loadFromFile("./files/images/number_8.png");
        sprites.emplace("number_8", sf::Sprite(texture9_8));
        sf::Texture texture10;
        texture10.loadFromFile("./files/images/pause.png");
        sprites.emplace("pause", sf::Sprite(texture10));
        sf::Texture texture11;
        texture11.loadFromFile("./files/images/play.png");
        sprites.emplace("play", sf::Sprite(texture11));
        sf::Texture texture12;
        texture12.loadFromFile("./files/images/tile_hidden.png");
        sprites.emplace("tile_hidden", sf::Sprite(texture12));
        sf::Texture texture13;
        texture13.loadFromFile("./files/images/tile_revealed.png");
        sprites.emplace("tile_revealed", sf::Sprite(texture13));
        sprites.emplace("tile_revealed_pause", sf::Sprite(texture13));

        while(!quit) {

            chrono::high_resolution_clock::time_point timerStart = chrono::high_resolution_clock::now();
            chrono::high_resolution_clock::time_point timerEnd;
            chrono::high_resolution_clock::time_point pausedTimer;
            chrono::duration<int> timer;
            chrono::duration<int> pausedTime;
            int finTime;
            int totPausedTime = 0;
            int tempPausedTime = 0;

            sf::RenderWindow window(sf::VideoMode((numCols * 32), (numRows * 32) + 100), "Minesweeper", sf::Style::Close);

            /// Set up empty board
            board.resize(numCols);
            for (int i = 0; i < board.size(); i++) {
                board.at(i).resize(numRows);
            }
            for (int i = 0; i < board.size(); i++) {
                for (int j = 0; j < board.at(i).size(); j++) {
                    Tile *tempTile = new Tile(i * 32, j * 32);
                    tempTile -> setUpTile(sprites);
                    board.at(i).at(j) = tempTile;
                }
            }

            /// Set up mines
            srand((unsigned) time(0));
            for (int i = 0; i < numMines; i++) {
                int rand_x = (rand() % numCols);
                int rand_y = (rand() % numRows);
                /// Ensure no double placement of mine
                if(board.at(rand_x).at(rand_y) -> mineTile) {
                    i--;
                } else {
                    board.at(rand_x).at(rand_y)->mine();
                }
            }
            bool minePressed = false;

            for (int i = 0; i < board.size(); i++) {
                for (int j = 0; j < board.at(i).size(); j++) {
                    board.at(i).at(j) -> addAdjacentTiles(board, sprites, i, j);
                }
            }

        /// Set up bottom bar

            /// Happy face/ Face_win/ Face_lose buttons
            sprites.at("face_happy").setPosition((((numCols) / 2.0f) * 32) - 32, 32 * (numRows + 0.5));
            sprites.at("face_lose").setPosition((((numCols) / 2.0f) * 32) - 32, 32 * (numRows + 0.5));
            sprites.at("face_win").setPosition((((numCols) / 2.0f) * 32) - 32, 32 * (numRows + 0.5));
            sprites.at("face_happy").setColor(sf::Color(255,255,255));
            sprites.at("face_lose").setColor(sf::Color::Transparent);
            sprites.at("face_win").setColor(sf::Color::Transparent);

            /// Debug button
            sprites.at("debug").setPosition((numCols * 32) - 304, 32 * (numRows + 0.5));
            bool gameDebug = false;

            /// Pause/Play buttons
            sprites.at("pause").setPosition((numCols * 32) - 240, 32 * (numRows + 0.5));
            sprites.at("play").setPosition((numCols * 32) - 240, 32 * (numRows + 0.5));
            bool paused = false;

            /// Leaderboard button
            sprites.at("leaderboard").setPosition((numCols * 32) - 176, 32 * (numRows + 0.5));
            bool LBOpen = false;

            /// Flag Counter
            // individual digit = 21x32 px
            int numFlags = 0;
            int counter = numMines - numFlags;
            sf::Sprite counterDigOne;
            sf::Sprite counterDigTwo;
            sf::Sprite counterDigThree;
            counterDigOne.setTexture(texture2);
            counterDigTwo.setTexture(texture2);
            counterDigThree.setTexture(texture2);
            if(counter >= 0 && counter < 100) {
                counterDigOne.setTextureRect(sf::IntRect((counter / 100) * 21, 0, 21, 32));
                counterDigTwo.setTextureRect(sf::IntRect((counter / 10) * 21, 0, 21, 32));
                counterDigThree.setTextureRect(sf::IntRect((counter % 10) * 21, 0, 21, 32));
            } else if(counter >= 100) {
                counterDigOne.setTextureRect(sf::IntRect((counter / 100) * 21, 0, 21, 32));
                counterDigTwo.setTextureRect(sf::IntRect((((counter % 100) / 10) * 21), 0, 21, 32));
                counterDigThree.setTextureRect(sf::IntRect((counter % 10) * 21, 0, 21, 32));
            }
            counterDigOne.setPosition(12, (32*(numRows + 0.5)) + 16);
            counterDigTwo.setPosition(33, (32 * (numRows + 0.5)) + 16);
            counterDigThree.setPosition(54, (32 * (numRows + 0.5)) + 16);

            /// Timer
            sf::Sprite timerMinOne, timerMinTwo;
            sf::Sprite timerSecOne, timerSecTwo;
            timerMinOne.setTexture(texture2);   timerMinTwo.setTexture(texture2);
            timerSecOne.setTexture(texture2);   timerSecTwo.setTexture(texture2);
            timerMinOne.setPosition((numCols*32)-97, (32*(numRows+0.5))+16);
            timerMinTwo.setPosition((numCols*32)-76, (32*(numRows+0.5))+16);
            timerSecOne.setPosition((numCols*32)-54, (32*(numRows+0.5))+16);
            timerSecTwo.setPosition((numCols*32)-33, (32*(numRows+0.5))+16);
            timerMinOne.setTextureRect(sf::IntRect(0,0,21,32));
            timerMinTwo.setTextureRect(sf::IntRect(0,0,21,32));
            timerSecOne.setTextureRect(sf::IntRect(0,0,21,32));
            timerSecTwo.setTextureRect(sf::IntRect(0,0,21,32));

            while (window.isOpen()) {
                sf::Event event;

                while (window.pollEvent(event)) {
                    /// Check if trying to quit
                    if (event.type == sf::Event::Closed) {
                        quit = true;
                        window.close();
                        break;
                    }
                    /// Check if mouse pressed on debug
                    if (event.type == sf::Event::MouseButtonPressed && sprites.at("debug").getGlobalBounds().contains((float) sf::Mouse::getPosition(window).x, (float) sf::Mouse::getPosition(window).y) && !paused && !minePressed && !LBOpen) {
                        gameDebug = !gameDebug;
                        if (gameDebug) {
                            sprites.at("debug").setColor(sf::Color(127, 127, 127));
                        } else {
                            sprites.at("debug").setColor(sf::Color(255, 255, 255));
                        }

                        for(int i = 0; i < board.size(); i++) {
                            for(int j = 0; j < board.at(i).size(); j++) {
                                if(board.at(i).at(j) -> mineTile) {
                                    board.at(i).at(j)->debug();
                                }
                            }
                        }
                    }
                    /// Check if mouse pressed on pause/play
                    if (event.type == sf::Event::MouseButtonPressed && sprites.at("pause").getGlobalBounds().contains((float) sf::Mouse::getPosition(window).x,(float) sf::Mouse::getPosition(window).y) && !minePressed && !LBOpen) {
                        paused = !paused;
                        if (paused) {
                            sprites.at("pause").setColor(sf::Color::Transparent);
                            sprites.at("play").setColor(sf::Color(255, 255, 255));
                        } else {
                            sprites.at("play").setColor(sf::Color::Transparent);
                            sprites.at("pause").setColor(sf::Color(255, 255, 255));
                        }

                        for(int i = 0; i < board.size(); i++) {
                            for(int j = 0; j < board.at(i).size(); j++) {
                                board.at(i).at(j)->pause();
                            }
                        }

                    }
                    /// Check if restart
                    if (event.type == sf::Event::MouseButtonPressed && sprites.at("face_happy").getGlobalBounds().contains((float) sf::Mouse::getPosition(window).x,(float) sf::Mouse::getPosition(window).y) && !LBOpen) {
                        gameDebug = !gameDebug;
                        sprites.at("debug").setColor(sf::Color(255, 255, 255));
                        sprites.at("face_happy").setColor(sf::Color(255,255,255));
                        sprites.at("face_lose").setColor(sf::Color::Transparent);
                        sprites.at("face_win").setColor(sf::Color::Transparent);
                        sprites.at("play").setColor(sf::Color::Transparent);
                        sprites.at("pause").setColor(sf::Color(255, 255, 255));
                        window.close();
                        break;
                    }
                    /// Check if leaderboard pressed
                    if (event.type == sf::Event::MouseButtonPressed && sprites.at("leaderboard").getGlobalBounds().contains((float) sf::Mouse::getPosition(window).x,(float) sf::Mouse::getPosition(window).y)) {
                        LBOpen = true;
                        if(!paused) {
                            for (int i = 0; i < board.size(); i++) {
                                for (int j = 0; j < board.at(i).size(); j++) {
                                    board.at(i).at(j)->pause();
                                }
                            }
                        }
                    }
                    /// Check if tile is pressed
                    for (int i = 0; i < board.size(); i++) {
                        for (int j = 0; j < board.at(i).size(); j++) {
                            if (event.type == sf::Event::MouseButtonPressed &&
                                board.at(i).at(j)->checkIfPosition((float) sf::Mouse::getPosition(window).x,(float) sf::Mouse::getPosition(window).y) && !paused && !minePressed && !LBOpen) {
                                if (event.mouseButton.button == sf::Mouse::Left) {
                                    board.at(i).at(j)->reveal();
                                    if(board.at(i).at(j) -> mineTile && !(board.at(i).at(j) -> flagTile)) {
                                        sprites.at("face_happy").setColor(sf::Color::Transparent);
                                        sprites.at("face_lose").setColor(sf::Color(255, 255, 255));
                                        for(int k = 0; k < board.size(); k++) {
                                            for(int l = 0; l < board.at(k).size(); l++) {
                                                if(board.at(k).at(l) -> mineTile && !(board.at(k).at(l) -> inDebug)) {
                                                    board.at(k).at(l)->debug();
                                                }
                                            }
                                        }
                                        minePressed = true;
                                    }
                                } else if (event.mouseButton.button == sf::Mouse::Right && (board.at(i).at(j) -> hidden)) {
                                    /// Change Counter
                                    if(board.at(i).at(j) -> flagTile) { numFlags--; } else { numFlags++; }
                                    counter = numMines - numFlags;
                                    if(counter >= 0 && counter < 100) {
                                        counterDigOne.setTextureRect(sf::IntRect((counter / 100) * 21, 0, 21, 32));
                                        counterDigTwo.setTextureRect(sf::IntRect((counter / 10) * 21, 0, 21, 32));
                                        counterDigThree.setTextureRect(sf::IntRect((counter % 10) * 21, 0, 21, 32));
                                    } else if(counter >= 100) {
                                        counterDigOne.setTextureRect(sf::IntRect((counter / 100) * 21, 0, 21, 32));
                                        counterDigTwo.setTextureRect(sf::IntRect((((counter % 100) / 10) * 21), 0, 21, 32));
                                        counterDigThree.setTextureRect(sf::IntRect((counter % 10) * 21, 0, 21, 32));
                                    } else {
                                        counterDigOne.setTextureRect(sf::IntRect(210, 0, 21, 32));
                                        counterDigTwo.setTextureRect(sf::IntRect((abs(counter) / 10) * 21, 0, 21, 32));
                                        counterDigThree.setTextureRect(sf::IntRect((abs(counter) % 10) * 21, 0, 21, 32));

                                    }
                                    /// Set Flag
                                    board.at(i).at(j)->flag();
                                }
                            }
                        }
                    }
                }
                /// Timer functionality
                if(!paused && !minePressed && !LBOpen) {
                    if(tempPausedTime > 0) {
                        totPausedTime += tempPausedTime;
                        tempPausedTime = 0;
                    }
                    timerEnd = chrono::high_resolution_clock::now();
                    timer = chrono::duration_cast<chrono::duration<int>>(timerEnd - timerStart);
                    finTime = timer.count() - totPausedTime;
                } else if(paused || LBOpen) {
                    pausedTimer = chrono::high_resolution_clock::now();
                    pausedTime = chrono::duration_cast<chrono::duration<int>>(pausedTimer - timerEnd);
                    tempPausedTime = pausedTime.count();
                }
                timerMinOne.setTextureRect(sf::IntRect(((finTime / 600)) * 21, 0, 21, 32));
                timerMinTwo.setTextureRect(sf::IntRect(((finTime / 60) % 10) * 21, 0, 21, 32));
                timerSecOne.setTextureRect(sf::IntRect(((finTime / 10) % 6) * 21, 0, 21, 32));
                timerSecTwo.setTextureRect(sf::IntRect((finTime % 10) * 21, 0, 21, 32));

                /// Check if win
                int countHidden = 0;
                for(int i = 0; i < board.size(); i++) {
                    for(int j = 0; j < board.at(i).size(); j++) {
                        if(!(board.at(i).at(j) -> mineTile)) {
                            if((board.at(i).at(j) -> hidden)) {
                                countHidden++;
                            }
                        }
                    }
                }
                if(countHidden == 0 && !paused && !LBOpen && counter >= 0) {
                    for(int i = 0; i < board.size(); i++) {
                        for(int j = 0; j < board.at(i).size(); j++) {
                            if(board.at(i).at(j) -> mineTile && !(board.at(i).at(j) -> flagTile)) {
                                board.at(i).at(j) -> flag();
                            }
                        }
                    }
                    if(counter >= 0 && counter < 100) {
                        counterDigOne.setTextureRect(sf::IntRect((0 / 100) * 21, 0, 21, 32));
                        counterDigTwo.setTextureRect(sf::IntRect((0 / 10) * 21, 0, 21, 32));
                        counterDigThree.setTextureRect(sf::IntRect((0 % 10) * 21, 0, 21, 32));
                    } else if(counter >= 100) {
                        counterDigOne.setTextureRect(sf::IntRect((0 / 100) * 21, 0, 21, 32));
                        counterDigTwo.setTextureRect(sf::IntRect((((0 % 100) / 10) * 21), 0, 21, 32));
                        counterDigThree.setTextureRect(sf::IntRect((0 % 10) * 21, 0, 21, 32));
                    }
                    if(gameDebug) {
                        for(int i = 0; i < board.size(); i++) {
                            for(int j = 0; j < board.at(i).size(); j++) {
                                if(board.at(i).at(j) -> mineTile) {
                                    board.at(i).at(j)->debug();
                                }
                            }
                        }
                        sprites.at("debug").setColor(sf::Color(255, 255, 255));
                    }
                    LB.addNewScore(name, finTime);
                    LBOpen = true;
                    paused = true;
                    sprites.at("face_win").setColor(sf::Color(255,255,255));
                    sprites.at("face_happy").setColor(sf::Color::Transparent);
                }

                window.clear(sf::Color(255, 255, 255));
                window.draw(sprites.at("face_happy"));
                window.draw(sprites.at("face_lose"));
                window.draw(sprites.at("face_win"));
                window.draw(sprites.at("debug"));
                window.draw(sprites.at("play"));
                window.draw(sprites.at("pause"));
                window.draw(sprites.at("leaderboard"));
                window.draw(counterDigOne);
                window.draw(counterDigTwo);
                window.draw(counterDigThree);
                window.draw(timerMinOne);
                window.draw(timerMinTwo);
                window.draw(timerSecOne);
                window.draw(timerSecTwo);
                for (int i = 0; i < board.size(); i++) {
                    for (int j = 0; j < board.at(i).size(); j++) {
                        board.at(i).at(j)->drawTile(window);
                    }
                }
                window.display();

                if(LBOpen) {
                    LB(numCols, numRows);
                    LBOpen = false;
                    pausedTimer = chrono::high_resolution_clock::now();
                    pausedTime = chrono::duration_cast<chrono::duration<int>>(pausedTimer - timerEnd);
                    tempPausedTime = pausedTime.count();
                    if(!paused) {
                        for (int i = 0; i < board.size(); i++) {
                            for (int j = 0; j < board.at(i).size(); j++) {
                                board.at(i).at(j)->pause();
                            }
                        }
                    }
                }
            }

        }
    }

    ~GameWindow(){
        for(int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board.at(i).size(); j++) {
                delete board.at(i).at(j);
            }
        }
    }

    private:
        map<string, sf::Sprite> sprites;
        vector<vector<Tile*>> board;
};
