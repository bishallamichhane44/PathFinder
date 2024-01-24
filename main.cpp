#include <SFML/Graphics.hpp>
#include<math.h>
#include <windows.h>
#include <iostream>
#include <cstring>
using namespace std;

#include "./includes/fonts.cpp"
#include "./includes/spriteManager.cpp"
#include "./includes/node.cpp"


//global variables
class window_settings{
    public:
    float windowWidth=1280;
    float windowHeight=720;
    int rows;
    int columns;
    float cellWidth;
    float cellHeight;

    sf::Color background= sf::Color::Transparent;
    sf::Color blockade = sf::Color::Black;
    sf::Color solution = sf::Color::Blue; 
    sf::Color iteration = sf::Color::Red;
    sf::Color ends = sf::Color::Green;
    sf::Color mouse = sf::Color::Yellow;
    sf::Color outline = sf::Color::White;


    string window_title="Path Finding Algorithm";


    bool block_inserting = true;
    bool normal = false;
    bool erasing = true;
    bool start= false;
    bool end=false;


    window_settings(int rows_n, int columns_n){
        rows = rows_n;
        columns = columns_n;
        cellWidth = windowWidth / columns;
        cellHeight = windowHeight / rows;
    }

};

window_settings Window(36*2,64*2);

class cell{
    public:
    int col_pos;
    int row_pos;
    int x_cod;
    int y_cod;

    bool isBlocked=false;
    bool isEnd=false;
    bool isStart=false;
    bool isunderCursor=false;
    bool isSoultion=false;
    bool isTraversed=false;

    sf::Color cellColor;

    cell(){
        if(isBlocked){
            cellColor = Window.blockade;
        }else if(isEnd || isStart){
            cellColor = Window.ends;
        }else if(isunderCursor){
            cellColor = Window.mouse;
        }else if(isTraversed){
            cellColor = Window.iteration;
        }else if(isSoultion){
            cellColor = Window.solution;
        }else{
            cellColor = Window.background;
        }
    }

    void update(){
        if(isBlocked){
            cellColor = Window.blockade;
        }else if( isStart){
            cellColor = sf::Color::Green;
        }else if( isEnd){
            cellColor = sf::Color::Magenta;
        }else if(isunderCursor){
            cellColor = Window.mouse;
        }else if(isTraversed){
            cellColor = Window.iteration;
        }else if(isSoultion){
            cellColor = Window.solution;
        }else{
            cellColor = Window.background;
        }


        if(Window.block_inserting){
            Window.mouse=Window.blockade;
        }else if(Window.start ){
            Window.mouse=sf::Color::Green;
        }else if( Window.end){
            Window.mouse=sf::Color::Magenta;
        }else if(Window.erasing){
            Window.mouse=sf::Color::Yellow;
        }
    }

};

class Grid{
    public:
    int rows=Window.rows;
    int columns=Window.columns;
    int total_cells=Window.rows*Window.columns;
    // Create a 2D vector with 3 rows and 12 columns, initialized to a default value (0 in this case)
   
    std::vector<std::vector<cell>> grid;

    Grid() {
        grid.resize(rows, std::vector<cell>(columns));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                // Initialize each cell with appropriate row and column positions
                grid[i][j].row_pos = i;
                grid[i][j].col_pos = j;
            }
        }
    }

    void update(sf::RenderWindow& window){
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for(int i=0;i<rows;i++){
            for(int j=0;j<columns;j++){
                int rowUnderMouse = mousePos.y / static_cast<int>(Window.cellHeight);
                int columnUnderMouse = mousePos.x / static_cast<int>(Window.cellWidth);
                grid[i][j].isunderCursor=false;
                if(i==rowUnderMouse && j==columnUnderMouse){
                    
                    grid[i][j].isunderCursor=true;

                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && Window.block_inserting==true){
                        grid[i][j].isBlocked=true;
                    }else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && Window.erasing==true){
                        grid[i][j].isBlocked=false;
                    }else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && Window.start==true){
                        grid[i][j].isStart=true;
                        Window.start=false;
                    }else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && Window.end==true){
                        grid[i][j].isEnd=true;
                        Window.end=false;
                    }
                    


                    
                

                }
             
                grid[i][j].update();

                sf::RectangleShape cell(sf::Vector2f(Window.cellWidth, Window.cellHeight));
                cell.setFillColor(grid[i][j].cellColor);
                cell.setOutlineThickness(1.0f);
                cell.setOutlineColor(sf::Color::White);
                cell.setPosition(j * Window.cellWidth, i * Window.cellHeight);
                window.draw(cell);
                
            }
            
        }

    

        
    }

};

Grid grid;

void handle_events(sf::RenderWindow &window)
{
    sf::Event event;

    while (window.pollEvent(event))
    {

        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // T -> To erase the blocks
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T)
        {
            Window.block_inserting = false;
            Window.erasing = true;
            Window.end = false;
            Window.start = false;
        }

        // B -> To insert the blocks
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
        {
            Window.block_inserting = true;
            Window.erasing = false;
            Window.end = false;
            Window.start = false;
        }

        // S -> To insert start block
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
        {
            Window.block_inserting = false;
            Window.erasing = false;
            Window.start = true;
            Window.end = false;
        }
        // E -> To insert end block
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
        {
            Window.block_inserting = false;
            Window.erasing = false;
            Window.start = false;
            Window.end = true;
        }
    }
}



int main(){
    sf::RenderWindow window;
    window.create(sf::VideoMode(1280, 720), Window.window_title, sf::Style::Titlebar | sf::Style::Close);
    
      while (window.isOpen()) {

        handle_events(window);         
        window.clear(sf::Color(240, 240, 240));
        grid.update(window);
        
        window.display();
    }



    return 0;
}