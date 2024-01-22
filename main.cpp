#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>
using namespace std;


// Font manager class
class UseFonts {
public:
    sf::Font font;

    UseFonts(const std::string& fontName, const std::string& fontFileLocation) {
        if (!font.loadFromFile(fontFileLocation)) {
            throw std::runtime_error("Failed to load font: " + fontName);
        }
    }
};
//Font Objects 
UseFonts colus("colus.otf", "fonts/colus.otf");
UseFonts roboto("roboto.otf", "fonts/roboto.ttf");


class spriteManager {
    private:
        int posX;
        int posY;

    public:
    sf::Sprite sprite;
    sf::Texture texture;
    

    spriteManager(const std::string& spriteName, const std::string& textureFileLocation , int posx=0, int posy=0) {
        posX=posx;
        posY=posy;

        if (!texture.loadFromFile(textureFileLocation)) {
            throw std::runtime_error("Failed to load texture: " + spriteName);
        }
        sprite.setTexture(texture);
        sprite.setPosition(posX, posY);
    }

    void updatePos(int posx, int posy) {
        posX = posx;
        posY = posy;
        sprite.setPosition(posX, posY);
    }

    void updateTexture(const std::string& textureFileLocation) {
        if (!texture.loadFromFile(textureFileLocation)) {
            throw std::runtime_error("Failed to load texture: " + textureFileLocation);
        }
        sprite.setTexture(texture);
    }

   



    void displaySprite(sf::RenderWindow& window) {
        window.draw(sprite);
        
    }
};

spriteManager cat("Cat","sprites/cat.png",10,100);





int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(1280, 720), "SFML window", sf::Style::Titlebar | sf::Style::Close);
   
  

   
    sf::Text text("Hello", roboto.font, 50);
    text.setFillColor(sf::Color::Blue);
    text.setPosition(300, 250);
    cout<<"Cat Position : "<<cat.sprite.getPosition().x<<endl;
    float catSpeed = 1; // Speed at which the cat moves across the screen

    while (window.isOpen()) {
        sf::Event event;
        // Update the position of the cat from left to right and vice versa
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2f catPosition = cat.sprite.getPosition();
        
        

        // If the cat reaches the right side of the screen, reverse the direction
        if (catPosition.x + cat.sprite.getGlobalBounds().width >= windowSize.x) {
            catSpeed = -catSpeed;
        }
        // If the cat reaches the left side of the screen, reverse the direction
        else if (catPosition.x <= 0) {
            catSpeed = -catSpeed;
        }

        // Update the cat's position
        cat.updatePos(catPosition.x + catSpeed, catPosition.y);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(240, 240, 240)); // Light grey background, close to white
        cat.displaySprite(window);
        window.draw(text);
        window.display();
    }

    return EXIT_SUCCESS;
}
