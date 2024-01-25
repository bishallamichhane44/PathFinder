#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <windows.h>
#include <iostream>
#include <stdio.h>
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

const int INFINITY = 10000;
int rows = 20;
int cols = 20;
int totalNodes = rows * cols;
int visitCount = 0;

class Cost
{
public:
    int rowNo, colNo, cost;
};

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
    Cost adj[3][3];

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

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                int r = rn + i;
                int c = cn + j;
                if (r >= 0 && c >= 0 && r < rows && c < cols)
                {
                    adj[i + 1][j + 1].rowNo = r;
                    adj[i + 1][j + 1].colNo = c;
                    if ((i == 1 || i == -1) && (j == 1 || j == -1))
                    {
                        adj[i + 1][j + 1].cost = 14;
                    }
                    else if (i == 0 && j == 0)
                    {
                        adj[i + 1][j + 1].cost = 0;
                    }
                    else
                    {
                        adj[i + 1][j + 1].cost = 10;
                    }
                }
                else
                {
                    adj[i + 1][j + 1].cost = INFINITY;
                }
            }
        }
    }
    void DrawCell(sf::RenderWindow &window)
    {
        window.draw(*cell);
    }
};

class Grid
{
public:
    Cell cells[70][70];
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
            tracker = 0;
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    cells[i][j].cell->setFillColor(sf::Color::Transparent);
                    cells[i][j].isSource = false;
                    cells[i][j].isDestination = false;
                    cells[i][j].isBlocked = false;
                    cells[i][j].isVisited = false;
                    cells[i][j].distanceFromSource = INFINITY;
                }
            }
        }

        // Drawing the Grid
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (!cells[i][j].isSource && !cells[i][j].isDestination && !cells[i][j].isBlocked && !cells[i][j].isVisited)
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
        if (rowUnderMouse >= 0 && rowUnderMouse < rows && columnUnderMouse >= 0 && columnUnderMouse < cols && !cells[rowUnderMouse][columnUnderMouse].isSource && !cells[rowUnderMouse][columnUnderMouse].isBlocked && !cells[rowUnderMouse][columnUnderMouse].isDestination && !cells[rowUnderMouse][columnUnderMouse].isVisited)
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
        int minValue = INFINITY;
        Cell *minNode = nullptr;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (!cells[i][j].isVisited && cells[i][j].distanceFromSource < minValue && !cells[i][j].isBlocked)
                {
                    minNode = &cells[i][j];
                    minValue = cells[i][j].distanceFromSource;
                }
            }
        }
        return minNode;
    }

    void displayFinal(Cell *dest)
    {
        while (!dest->parent->isSource)
        {
            dest = dest->parent;
            dest->cell->setFillColor(sf::Color::Cyan);
        }
    }

    void dijkastra(sf::RenderWindow &window)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                Cell *nearest = getNearestNode();
                nearest->isVisited = true;
                visitCount++;
                nearest->cell->setFillColor(sf::Color::Magenta);
                if (nearest->isSource)
                {
                    nearest->cell->setFillColor(sf::Color::Green);
                }
                if (nearest->isDestination)
                {
                    nearest->cell->setFillColor(sf::Color::Red);
                    displayFinal(nearest);
                    sf::sleep(sf::seconds(1.0f));
                    return;
                }

                for (int k = 0; k < 3; k++)
                {
                    for (int l = 0; l < 3; l++)
                    {

                        Cell &c1 = cells[nearest->adj[k][l].rowNo][nearest->adj[k][l].colNo];
                        if (nearest->adj[k][l].cost != INFINITY && (c1.distanceFromSource > (nearest->distanceFromSource + nearest->adj[k][l].cost)))
                        {
                            c1.distanceFromSource = nearest->distanceFromSource + nearest->adj[k][l].cost;
                            c1.parent = nearest;
                        }
                    }
                }
            }
        }
    }
};

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
            g1.dijkastra(window);
        }

        window.clear(sf::Color(240, 240, 240)); // Light grey background, close to white
        g1.displayGrid(window);

        window.display();
    }

    return EXIT_SUCCESS;
}
