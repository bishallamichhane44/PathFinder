#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>
using namespace std;


// Font manager class
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

//Sprite Objects "I have used cat object as a sample object".
spriteManager cat("Cat","sprites/cat.png",10,100);



//keep track of last added cell to avoid duplicate addition.
static int lastcol=0;
static int lastrow=0;

void displayGrid(sf::RenderWindow& window, int rows, int columns, bool colorToggle, bool & clear,bool & undo,bool & initials) {

    //code to generate the grid.
    float windowWidth = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);
    float cellWidth = windowWidth / columns;
    float cellHeight = windowHeight / rows;

    sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
    cell.setFillColor(sf::Color::Transparent);
    cell.setOutlineThickness(1.0f);
    cell.setOutlineColor(sf::Color::White);

    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            cell.setPosition(column * cellWidth, row * cellHeight);
            window.draw(cell);
        }
    }


/*     //Code to draw the red indicator under mouse pointer.
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int rowUnderMouse = mousePos.y / static_cast<int>(cellHeight);
    int columnUnderMouse = mousePos.x / static_cast<int>(cellWidth);

    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            if (row == rowUnderMouse && column == columnUnderMouse) {
                cell.setFillColor(sf::Color::Red);
            } else {
                cell.setFillColor(sf::Color::Transparent);
            }
            cell.setPosition(column * cellWidth, row * cellHeight);
            window.draw(cell);
        }
    }
 */



// Code to color the cell green under the mouse cursor and black for cells selected with click and drag
sf::Vector2i mousePos = sf::Mouse::getPosition(window);
int rowUnderMouse = mousePos.y / static_cast<int>(cellHeight);
int columnUnderMouse = mousePos.x / static_cast<int>(cellWidth);

bool mouseHeld = sf::Mouse::isButtonPressed(sf::Mouse::Left);
static bool dragging = false;
static std::vector<sf::Vector2i> selectedCells;
static int startx=-2;
static int starty=-2;
static int endx=-2;
static int endy=-2; 
static int tracker=0;



// Variable to toggle cell selection coloring


    if(clear==true){
        cout<<"Screen Cleared"<<endl;
        selectedCells.clear();
        clear=false;
    }
    if(undo==true){
        
        if(selectedCells.size()>0){
            selectedCells.pop_back();
            cout<<"Undo"<<endl;
        }
     
        undo=false;
    }



// Start dragging
if (mouseHeld && !dragging && colorToggle ) {
    dragging = true;
}


// While dragging, add cells to the selection
if (dragging && mouseHeld && colorToggle) {
   
    
    if (rowUnderMouse != lastrow || columnUnderMouse != lastcol) {
        if(initials){
            if(tracker==0){
                startx=columnUnderMouse;
                starty=rowUnderMouse;
            }else{
                endx=columnUnderMouse;
                endy=rowUnderMouse;

            }
            
            
            tracker++;
            if(tracker>=2){
                initials=false;
            }
            
            
        }else{
            selectedCells.push_back({columnUnderMouse, rowUnderMouse});

        }
        
        lastcol=columnUnderMouse;
        lastrow=rowUnderMouse;
        
    }

    
    
}

// Stop dragging
if (!mouseHeld && dragging && colorToggle) {
    dragging = false;
}








// Draw cells
for (int row = 0; row < rows; ++row) {
    for (int column = 0; column < columns; ++column) {
        // Check if the current cell is being dragged over
        bool cellSelected = std::find(selectedCells.begin(), selectedCells.end(), sf::Vector2i(column, row)) != selectedCells.end();
       
       if(row == starty && column == startx){
            cell.setFillColor(sf::Color::Yellow); }

            else if(row == endy && column == endx){
                cell.setFillColor(sf::Color::Red);

            }
       else{ if (row == rowUnderMouse && column == columnUnderMouse && colorToggle) {
            cell.setFillColor(sf::Color::Blue); // Green color for cell under mouse
        } else if (row == rowUnderMouse && column == columnUnderMouse && !colorToggle ) {
            cell.setFillColor(sf::Color::Green); // Black color for selected cells if toggle is on
        }else if (cellSelected ) {
            cell.setFillColor(sf::Color::Black); // Black color for selected cells if toggle is on
        } else {
            cell.setFillColor(sf::Color::Transparent);
        }}

      

        cell.setPosition(column * cellWidth, row * cellHeight);
        window.draw(cell);
    }
}


}



int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(1280, 720), "SFML window", sf::Style::Titlebar | sf::Style::Close);
   
  

   
    /* sf::Text text("Hello", roboto.font, 50);
    text.setFillColor(sf::Color::Blue);
    text.setPosition(300, 250); */
    
   
    static bool colorToggle = true;
    bool clear=false;
    bool undo=false;
    bool initials=true;


    while (window.isOpen()) {
        sf::Event event;
        // Update the position of the cat from left to right and vice versa
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2f catPosition = cat.sprite.getPosition();
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

             if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T) {
            colorToggle = !colorToggle; // Toggle the coloring feature
            }
            
            //Toogle screen clear
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
                clear=true;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K) {
                undo=true;
            }
        }



    
   

        window.clear(sf::Color(240, 240, 240)); // Light grey background, close to white
        displayGrid(window,36,64,colorToggle,clear,undo,initials);
       /*  cat.displaySprite(window);  */

        

        
       
        window.display();
    }

    return EXIT_SUCCESS;
}
