/*
** EPITECH PROJECT, 2024
** B-PDG-300-PAR-3-1-PDGRUSH3-guillaume.houriez
** File description:
** display
*/

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <pwd.h>
#include <ctime>

class MyGkrellM {
    public:
        MyGkrellM();
        ~MyGkrellM();
        void run();
    protected:
    private:
        sf::RenderWindow window;
        sf::Font font;
        sf::Text text;
        sf::RectangleShape scrollbar;
        float scrollPosition;
        float textHeight;
        bool isScrolling;
        float offset;
        void handleEvents();
        void update();
        void render();
        float getNetworkLoad();
};

#endif /* !DISPLAY_HPP_ */
