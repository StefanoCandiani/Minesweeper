//
// Created by Stefano Candiani on 4/21/24.
//
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <map>
using namespace std;

struct Leaderboard {

    void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
                       textRect.top  + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

    void addNewScore(string name, int time) {
        leadeboard.emplace(time, name + '*');
        ofstream writeFile;
        writeFile.open("./files/leaderboard.txt", ios_base::app);
        string seconds = to_string(time % 60);
        string minutes = to_string(time / 60);
        if(seconds.size() < 2) { seconds = '0' + seconds; }
        if(minutes.size() < 2) { minutes = '0' + minutes; }
        writeFile << "\n" + minutes + ":" + seconds + ", " + name;
    }

    void getLeaderboard() {
        ifstream readFile;
        readFile.open("./files/leaderboard.txt");
        string buffer, time, name;
        int divideIndex, sec;
        while(getline(readFile, buffer)) {
            if(buffer.size() == 0) { continue; }
            divideIndex = buffer.find(",");
            time = buffer.substr(0, divideIndex);
            name = buffer.substr(divideIndex + 2, buffer.size());
            sec = (60 * stoi(time.substr(0, time.find(":")))) + (stoi(time.substr(time.find(":") + 1, time.size())));
            leadeboard.emplace(sec, name);
        }
        readFile.close();
    }

    void operator()(int numCols, int numRows) {

        getLeaderboard();

        sf::RenderWindow LBwindow(sf::VideoMode((numCols * 16), (numRows * 16) + 50), "Leaderboard", sf::Style::Close);

        sf::Font LBFont;
        LBFont.loadFromFile("./files/font.ttf");

        sf::Text LBTitle("LEADERBOARD", LBFont, 20);
        LBTitle.setFillColor(sf::Color(255,255,255));
        LBTitle.setStyle(sf::Text::Underlined | sf::Text::Bold);
        setText(LBTitle, ((float)LBwindow.getSize().x)/2,(((float)LBwindow.getSize().y)/2)-120);

        int count = 1;
        string player = "";
        auto iter = leadeboard.begin();
        for(; iter != leadeboard.end(); iter++) {
            if(count <= 5) {
                string seconds = to_string((iter->first) % 60);
                if (seconds.size() < 2) { seconds = "0" + seconds; }
                string time = to_string((iter->first) / 60) + ":" + seconds;
                player += to_string(count) + ".\t" + time + "\t" + iter->second + "\n\n";
                count++;
            }
        }

        sf::Text LBPlace(player, LBFont, 18);
        setText(LBPlace, ((float) LBwindow.getSize().x) / 2,
                (((float) LBwindow.getSize().y) / 2) + (20));
        LBPlace.setFillColor(sf::Color(255, 255, 255));
        LBPlace.setStyle(sf::Text::Bold);

        while (LBwindow.isOpen()) {
            sf::Event event;

            while (LBwindow.pollEvent(event)) {
                /// Check if trying to quit
                if (event.type == sf::Event::Closed /*|| event.key.code == sf::Keyboard::Escape*/) {
                    LBwindow.close();
                    break;
                }

            }

            LBwindow.clear(sf::Color(0, 0, 255));
            LBwindow.draw(LBTitle);
            LBwindow.draw(LBPlace);
            LBwindow.display();
        }
    }

    private:
        map<int, string> leadeboard;

};