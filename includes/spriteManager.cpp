#include<iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>

class spriteManager {
    private:
        int posX;
        int posY;

    public:
    sf::Sprite sprite;
    sf::Texture texture;
    

    spriteManager(const std::string& spriteName, const std::string& textureFileLocation , int posx=0, int posy=0) {
        //This function will create a sprite with the name passed as the parameter and add the texture given as parameter.
        //posx,posy refers to the position of the sprite in the window.
        posX=posx;
        posY=posy;

        if (!texture.loadFromFile(textureFileLocation)) {
            throw std::runtime_error("Failed to load texture: " + spriteName);
        }
        sprite.setTexture(texture);
        sprite.setPosition(posX, posY);
    }

    void updatePos(int posx, int posy) {
        //Code to update the position of the sprite.
        posX = posx;
        posY = posy;
        sprite.setPosition(posX, posY);
    }

    void updateTexture(const std::string& textureFileLocation) {
        //This code will update the texture of the sprite.
        if (!texture.loadFromFile(textureFileLocation)) {
            throw std::runtime_error("Failed to load texture: " + textureFileLocation);
        }
        sprite.setTexture(texture);
    }

   



    void displaySprite(sf::RenderWindow& window) {
        //code to draw the sprite on the window passed as the parameter.
        window.draw(sprite);
        
    }
};