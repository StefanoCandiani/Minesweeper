//
// Created by Stefano Candiani on 4/10/24.
//
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

struct IntroCard{

    void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
                       textRect.top  + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

    string operator()(int numCols, int numRows) {

        string retVal = "NoProperNameProvided";

        /// Window
        sf::RenderWindow window(sf::VideoMode((numCols*32), (numRows*32)+100), "Minesweeper", sf::Style::Close);

        /// Font for intro card
        sf::Font introFont;
        introFont.loadFromFile("./files/font.ttf");

        /// "Welcome to Minesweeper!" card
        sf::Text welcome("WELCOME TO MINESWEEPER!", introFont, 24);
        welcome.setFillColor(sf::Color(255,255,255));
        welcome.setStyle(sf::Text::Underlined | sf::Text::Bold);
        setText(welcome, (window.getSize().x)/2, ((window.getSize().y)/2)-150);

        /// "Enter your name card:" card
        sf::Text namePrompt("Enter your name:", introFont, 20);
        namePrompt.setFillColor(sf::Color(255,255,255));
        setText(namePrompt, (window.getSize().x)/2, ((window.getSize().y)/2)-75);

        /// Name typing printing
        string name = "|";
        int index = 0;

        while(window.isOpen()) {

            sf::Text namePrint(name, introFont, 18);
            namePrint.setFillColor(sf::Color(191, 191, 64));
            setText(namePrint, (window.getSize().x)/2, ((window.getSize().y)/2)-45);

            sf::Event event;
            while(window.pollEvent(event)) {
                if (event.type == sf::Event::Closed /*|| event.key.code == sf::Keyboard::Escape*/) {
                    window.close();
                    break;
                }
                if(event.key.code == sf::Keyboard::Enter && name.size() > 1) {
                    retVal = name.substr(0,name.size()-1);
                    return retVal;
                }
                if(event.type == sf::Event::TextEntered) {
                    char letter = static_cast<char>(event.text.unicode);
                    if(isalpha(letter) && name.size() < 11) {
                        if(index == 0) { letter = toupper(letter); }
                        else { letter = tolower(letter); }
                        string tempName = name.substr(0, name.size() - 1);
                        tempName += letter;
                        tempName += '|';
                        name = tempName;
                        index++;
                    }
                    if(event.text.unicode == 8 && name.size() > 1) {
                        name = name.substr(0, name.size()-2);
                        name += '|';
                        index--;
                    }
                }
            }

            window.clear(sf::Color(0, 0, 255));
            window.draw(welcome);
            window.draw(namePrompt);
            window.draw(namePrint);
            window.display();
        }
        return retVal;
    }

};

