#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>
using namespace std;

// Font manager class
class UseFonts
{
    // Made to make loading and using fonts easier.
public:
    sf::Font font;

    UseFonts(const std::string &fontName, const std::string &fontFileLocation)
    {
        if (!font.loadFromFile(fontFileLocation))
        {
            throw std::runtime_error("Failed to load font: " + fontName);
        }
    }
};
// Font Objects
UseFonts colus("colus.otf", "fonts/colus.otf");
UseFonts roboto("roboto.otf", "fonts/roboto.ttf");

bool startAlgo = false;

class spriteManager
{
private:
    int posX;
    int posY;

public:
    sf::Sprite sprite;
    sf::Texture texture;

    spriteManager(const std::string &spriteName, const std::string &textureFileLocation, int posx = 0, int posy = 0)
    {
        // This function will create a sprite with the name passed as the parameter and add the texture given as parameter.
        // posx,posy refers to the position of the sprite in the window.
        posX = posx;
        posY = posy;

        if (!texture.loadFromFile(textureFileLocation))
        {
            throw std::runtime_error("Failed to load texture: " + spriteName);
        }
        sprite.setTexture(texture);
        sprite.setPosition(posX, posY);
    }

    void updatePos(int posx, int posy)
    {
        // Code to update the position of the sprite.
        posX = posx;
        posY = posy;
        sprite.setPosition(posX, posY);
    }

    void updateTexture(const std::string &textureFileLocation)
    {
        // This code will update the texture of the sprite.
        if (!texture.loadFromFile(textureFileLocation))
        {
            throw std::runtime_error("Failed to load texture: " + textureFileLocation);
        }
        sprite.setTexture(texture);
    }

    void displaySprite(sf::RenderWindow &window)
    {
        // code to draw the sprite on the window passed as the parameter.
        window.draw(sprite);
    }
};

const int INFINITY = 10000;
int costMatrix[10][10][10][10];

class Cell
{
public:
    sf::RectangleShape *cell;
    bool isSource;
    bool isDestination;
    bool isBlocked;
    bool isVisited;
    int distanceFromSource;
    int rowNo, colNo;
    Cell *parent;

    void createCell(float cellWidth, float cellHeight, int posX, int posY, int rn, int cn)
    {
        cell = new sf::RectangleShape(sf::Vector2f(cellWidth, cellHeight));
        isSource = false;
        isDestination = false;
        isBlocked = false;
        isVisited = false;
        distanceFromSource = INFINITY;
        rowNo = rn;
        colNo = cn;
        cell->setPosition(posX, posY);
        cell->setFillColor(sf::Color::Transparent);
        cell->setOutlineThickness(1.0f);
        cell->setOutlineColor(sf::Color::White);
        parent = this;
    }
    void DrawCell(sf::RenderWindow &window)
    {
        window.draw(*cell);
    }
};

class Grid
{
public:
    Cell cells[10][10];
    int rows;
    int cols;
    float cellWidth, cellHeight;
    int tracker;

    void createGrid(int row, int col, float cellW, float cellH)
    {
        rows = row;
        cols = col;
        cellWidth = cellW;
        cellHeight = cellH;
        tracker = 0;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cells[i][j].createCell(cellWidth, cellHeight, j * cellWidth, i * cellHeight, i, j);
            }
        }
    }

    void displayGrid(sf::RenderWindow &window)
    {
        // For resetting the app
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    cells[i][j].cell->setFillColor(sf::Color::Transparent);
                    cells[i][j].isSource = false;
                    cells[i][j].isDestination = false;
                    cells[i][j].isBlocked = false;
                    cells[i][j].isVisited = false;
                    tracker = 0;
                }
            }
        }

        // Drawing the Grid
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (!cells[i][j].isSource && !cells[i][j].isDestination && !cells[i][j].isBlocked)
                {
                    cells[i][j].cell->setFillColor(sf::Color::Transparent);
                }
                cells[i][j].DrawCell(window);
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int rowUnderMouse = mousePos.y / cellHeight;
        int columnUnderMouse = mousePos.x / cellWidth;

        // For hovering effect
        if (rowUnderMouse >= 0 && rowUnderMouse < rows && columnUnderMouse >= 0 && columnUnderMouse < cols && !cells[rowUnderMouse][columnUnderMouse].isSource && !cells[rowUnderMouse][columnUnderMouse].isBlocked && !cells[rowUnderMouse][columnUnderMouse].isDestination)
        {
            cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color::Blue);
        }

        // For mouse Clicks
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && rowUnderMouse >= 0 && rowUnderMouse < rows && columnUnderMouse >= 0 && columnUnderMouse < cols)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // For starting cell
            if (tracker == 0)
            {
                cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color::Green);
                cells[rowUnderMouse][columnUnderMouse].isSource = true;
                cells[rowUnderMouse][columnUnderMouse].distanceFromSource = 0;
                tracker++;
            }

            // For destination cell
            else if (tracker == 1 && !cells[rowUnderMouse][columnUnderMouse].isSource)
            {
                cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color::Red);
                cells[rowUnderMouse][columnUnderMouse].isDestination = true;
                tracker++;
            }

            // For Blocking cell
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && !cells[rowUnderMouse][columnUnderMouse].isSource && !cells[rowUnderMouse][columnUnderMouse].isDestination && !cells[rowUnderMouse][columnUnderMouse].isBlocked)
            {
                cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color::Black);
                cells[rowUnderMouse][columnUnderMouse].isBlocked = true;
            }

            // For removing Blockage
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && cells[rowUnderMouse][columnUnderMouse].isBlocked)
            {
                cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color::Transparent);
                cells[rowUnderMouse][columnUnderMouse].isBlocked = false;
            }
        }

        // Draw Grid after various events
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cells[i][j].DrawCell(window);
            }
        }
    }
    Cell *getNearestNode()
    {
        int minValue = cells[0][0].distanceFromSource;
        Cell *minNode = &cells[0][0];
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (!cells[i][j].isVisited && cells[i][j].distanceFromSource < minValue)
                {
                    minNode = &cells[i][j];
                }
            }
        }
        return minNode;
    }

    void dijkastra()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                Cell *nearest = getNearestNode();
                nearest->isVisited = true;
                nearest->cell->setFillColor(sf::Color::Magenta);
                for (int k = 0; k < rows; k++)
                {
                    for (int l = 0; l < cols; l++)
                    {
                        if (costMatrix[nearest->rowNo][nearest->colNo][k][l] != INFINITY &&
                            cells[k][l].distanceFromSource > nearest->distanceFromSource + costMatrix[nearest->rowNo][nearest->colNo][k][l])
                        {
                            cells[k][l].distanceFromSource = nearest->distanceFromSource + costMatrix[nearest->rowNo][nearest->colNo][k][l];
                            cells[k][l].parent = nearest;
                        }
                    }
                }
            }
        }
    }
};

int rows = 10;
int cols = 10;
int totalNodes = rows * cols;
void init(int nodes)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < rows; k++)
            {
                for (int l = 0; l < rows; l++)
                {
                    if (i == k && j == k)
                    {
                        costMatrix[i][j][k][l] = 0;
                    }
                    else if (i + 1 == k && j == l || i - 1 == k && j == l || i == k && j + 1 == l || i == k && j - 1 == l)
                    {
                        costMatrix[i][j][k][l] = 10;
                    }
                    else if (i + 1 == k && j + 1 == l || i + 1 == k && j - 1 == l || i - 1 == k && j + 1 == l || i - 1 == k && j - 1 == l)
                    {
                        costMatrix[i][j][k][l] = 14;
                    }
                    else
                    {
                        costMatrix[i][j][k][l] = INFINITY;
                    }
                }
            }
        }
    }
}

int main()
{
    float windowWidth = 600;
    float windowHeight = 600;
    sf::RenderWindow window;
    window.create(sf::VideoMode(windowWidth, windowHeight), "SFML window", sf::Style::Titlebar | sf::Style::Close);

    float cellWidth = windowWidth / rows;
    float cellHeight = windowHeight / cols;

    Grid g1;
    g1.createGrid(rows, cols, cellWidth, cellHeight);
    init(rows * cols);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        {
            g1.dijkastra();
            startAlgo = false;
        }

        window.clear(sf::Color(240, 240, 240)); // Light grey background, close to white
        g1.displayGrid(window);

        window.display();
    }

    return EXIT_SUCCESS;
}