#include<iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>
class UseFonts {
    //Made to make loading and using fonts easier.
public:
    sf::Font font;

    UseFonts(const std::string& fontName, const std::string& fontFileLocation) {
        if (!font.loadFromFile(fontFileLocation)) {
            throw std::runtime_error("Failed to load font: " + fontName);
        }
    }
};
