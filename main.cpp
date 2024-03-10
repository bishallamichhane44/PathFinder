#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <windows.h>
#include <iostream>
#include <cmath>
using namespace std;

//colors 
// background = sf::Color(0xDC, 0xF2, 0xF1)
// blockade = sf::Color(0x0F, 0x10, 0x35) 
// path = sf::Color(0xFF, 0xBA ,0x86)
// trials = sf::Color(0x7F, 0xC7 ,0xD9) 
// start = sf::Color(0x17, 0x6B ,0x87) 176B87
// end = sf::Color(0xD2, 0x45 ,0x45) D24545 
// cursor = sf::Color(0x61, 0x67 ,0x7A) 61677A



const int INF = 10000;
int rows = 36;
int cols = 64;

const float windowWidth = 1280;
const float windowHeight = 720;

const float cellWidth = windowWidth / cols;
const float cellHeight = windowHeight / rows;

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

class Cost
{
public:
    int rowNo, colNo, cost;
};

class Cell
{
public:
    sf::RectangleShape *cell = new sf::RectangleShape(sf::Vector2f(cellWidth, cellHeight));
    bool isSource = false;
    bool isDestination = false;
    bool isBlocked = false;
    bool isVisited = false;
    int distanceFromSource = INF, distanceFromDest = INF;
    int rowNo, colNo;
    Cell *parent;
    Cost adj[3][3];

    void createCell(float cellWidth, float cellHeight, int posX, int posY, int rn, int cn)
    {
        distanceFromSource = INF;
        distanceFromDest = INF;
        rowNo = rn;
        colNo = cn;
        cell->setPosition(posX, posY);
        if (isVisited)
        {
            cell->setFillColor(sf::Color(0xDC, 0xF2, 0xF1));
            isVisited = false;
        }
        cell->setOutlineThickness(0.5f);
        cell->setOutlineColor(sf::Color(0x2D, 0x32, 0x50,75)); // alternative black
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
                    adj[i + 1][j + 1].cost = INF;
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
    Cell *sourceCell = nullptr;
    Cell *destCell = nullptr;
    int rows;
    int cols;
    int tracker = 0;

    void initializeGrid(int row, int col, sf::RenderWindow &window)
    {
        rows = row;
        cols = col;

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {

                if (cells[i][j].isVisited)
                {
                    cells[i][j].cell->setFillColor(sf::Color(0xDC, 0xF2, 0xF1));
                    cells[i][j].isVisited = false;
                }
                if (cells[i][j].isBlocked)
                {
                    cells[i][j].cell->setFillColor(sf::Color(0x0F, 0x10, 0x35));
                    cells[i][j].isVisited = false;
                }
                
                cells[i][j].createCell(cellWidth, cellHeight, j * cellWidth, i * cellHeight, i, j);
                cells[i][j].DrawCell(window);
            }
        }
        window.display();
    }

    void drawGrid(sf::RenderWindow &window)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cells[i][j].DrawCell(window);
            }
        }
    }

    int calcDistanceFromDestination(int r, int c)
    {
        int value = sqrt(pow(destCell->rowNo - r, 2) + pow(destCell->colNo - c, 2)) * 10;
        return (value);
    }

    void restartForAlgo(sf::RenderWindow &window)
    {
        initializeGrid(rows, cols, window);

        cells[sourceCell->rowNo][sourceCell->colNo].isSource = true;
        cells[sourceCell->rowNo][sourceCell->colNo].cell->setFillColor(sf::Color(0x17, 0x6B ,0x87));
        cells[sourceCell->rowNo][sourceCell->colNo].distanceFromSource = 0;
        cells[sourceCell->rowNo][sourceCell->colNo].distanceFromDest = calcDistanceFromDestination(sourceCell->rowNo, sourceCell->colNo);

        cells[destCell->rowNo][destCell->colNo].isDestination = true;
        cells[destCell->rowNo][destCell->colNo].cell->setFillColor(sf::Color(0xD2, 0x45 ,0x45));  //ending
        cells[destCell->rowNo][destCell->colNo].distanceFromDest = 0;
    }

    void clearAll()
    {
        tracker = 0;
        sourceCell = nullptr;
        destCell = nullptr;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cells[i][j].cell->setFillColor(sf::Color(0xDC, 0xF2, 0xF1));
                cells[i][j].isSource = false;
                cells[i][j].isDestination = false;
                cells[i][j].isBlocked = false;
                cells[i][j].isVisited = false;
                cells[i][j].distanceFromSource = INF;
                cells[i][j].distanceFromDest = INF;
                cells[i][j].parent = &cells[i][j];
            }
        }
    }

    void displayGrid(sf::RenderWindow &window)
    {
        // For resetting the app
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            clearAll();
        }

        // Drawing the Grid
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (!cells[i][j].isSource && !cells[i][j].isDestination && !cells[i][j].isBlocked && !cells[i][j].isVisited)
                {
                    cells[i][j].cell->setFillColor(sf::Color(0xDC, 0xF2, 0xF1));
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
            cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color(0x61, 0x67 ,0x7A));
        }

        // For mouse Clicks
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && rowUnderMouse >= 0 && rowUnderMouse < rows && columnUnderMouse >= 0 && columnUnderMouse < cols)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // For starting cell
            if (tracker == 0)
            {
                cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color(0x17, 0x6B ,0x87)); //starting cell
                cells[rowUnderMouse][columnUnderMouse].isSource = true;
                cells[rowUnderMouse][columnUnderMouse].distanceFromSource = 0;
                sourceCell = &cells[rowUnderMouse][columnUnderMouse];
                tracker++;
            }

            // For destination cell
            else if (tracker == 1 && !cells[rowUnderMouse][columnUnderMouse].isSource)
            {
                cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color(0xD2, 0x45 ,0x45)); //ending
                cells[rowUnderMouse][columnUnderMouse].isDestination = true;
                cells[rowUnderMouse][columnUnderMouse].distanceFromDest = 0;
                destCell = &cells[rowUnderMouse][columnUnderMouse];
                sourceCell->distanceFromDest = calcDistanceFromDestination(sourceCell->rowNo, sourceCell->colNo);
                tracker++;
            }

            // For Blocking cell
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && !cells[rowUnderMouse][columnUnderMouse].isSource && !cells[rowUnderMouse][columnUnderMouse].isDestination && !cells[rowUnderMouse][columnUnderMouse].isBlocked)
            {
                cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color(0x0F, 0x10, 0x35));
                cells[rowUnderMouse][columnUnderMouse].isBlocked = true;
            }

            // For removing Blockage
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && cells[rowUnderMouse][columnUnderMouse].isBlocked)
            {
                cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color(0xDC, 0xF2, 0xF1));
                cells[rowUnderMouse][columnUnderMouse].isBlocked = false;
            }
        }

        // Draw Grid after various events
        drawGrid(window);
    }

    Cell *getNearestNodeForAStar()
    {
        int minValue = INF;
        Cell *minNode = nullptr;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (!cells[i][j].isVisited && (cells[i][j].distanceFromSource + cells[i][j].distanceFromDest) < minValue && !cells[i][j].isBlocked)
                {
                    minNode = &cells[i][j];
                    minValue = cells[i][j].distanceFromSource + cells[i][j].distanceFromDest;
                }
            }
        }
        return minNode;
    }

    Cell *getNearestNodeForDijkastra()
    {
        int minValue = INF;
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
            dest->cell->setFillColor(sf::Color(0xFF, 0xBA ,0x86));
        }
    }

    void dijkastra(sf::RenderWindow &window)
    {
        restartForAlgo(window);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                Cell *nearest = getNearestNodeForDijkastra();
                nearest->isVisited = true;
                nearest->cell->setFillColor(sf::Color(0x7F, 0xC7 ,0xD9));  //trials

                drawGrid(window);
                window.display();
                sf::sleep(sf::seconds(0.01f));

                if (nearest->isSource)
                {
                    nearest->cell->setFillColor(sf::Color(0x17, 0x6B ,0x87)); //starting
                }
                if (nearest->isDestination)
                {
                    nearest->cell->setFillColor(sf::Color(0xD2, 0x45 ,0x45)); //ending
                    displayFinal(nearest);
                    return;
                }

                for (int k = 0; k < 3; k++)
                {
                    for (int l = 0; l < 3; l++)
                    {

                        Cell &c1 = cells[nearest->adj[k][l].rowNo][nearest->adj[k][l].colNo];
                        if (nearest->adj[k][l].cost != INF && (c1.distanceFromSource > (nearest->distanceFromSource + nearest->adj[k][l].cost)))
                        {
                            c1.distanceFromSource = nearest->distanceFromSource + nearest->adj[k][l].cost;
                            c1.parent = nearest;
                        }
                    }
                }
            }
        }
    }

    void aStar(sf::RenderWindow &window)
    {
        restartForAlgo(window);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                Cell *nearest = getNearestNodeForAStar();
                nearest->isVisited = true;
                nearest->cell->setFillColor(sf::Color(0x7F, 0xC7 ,0xD9)); //trials
                drawGrid(window);
                window.display();
                sf::sleep(sf::seconds(0.01f));
                if (nearest->isSource)
                {
                    nearest->cell->setFillColor(sf::Color(0x17, 0x6B ,0x87)); //starting
                }
                if (nearest->isDestination)
                {
                    nearest->cell->setFillColor(sf::Color(0xD2, 0x45 ,0x45)); //ending
                    displayFinal(nearest);
                    return;
                }

                for (int k = 0; k < 3; k++)
                {
                    for (int l = 0; l < 3; l++)
                    {

                        Cell &c1 = cells[nearest->adj[k][l].rowNo][nearest->adj[k][l].colNo];
                        if (nearest->adj[k][l].cost != INF && (c1.distanceFromSource > (nearest->distanceFromSource + nearest->adj[k][l].cost)))
                        {
                            c1.distanceFromSource = nearest->distanceFromSource + nearest->adj[k][l].cost;
                            c1.distanceFromDest = calcDistanceFromDestination(nearest->adj[k][l].rowNo, nearest->adj[k][l].colNo);
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
    sf::RenderWindow window;
    window.create(sf::VideoMode(windowWidth, windowHeight), "SFML window", sf::Style::Titlebar | sf::Style::Close);

    Grid g1;
    g1.initializeGrid(rows, cols, window);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            g1.dijkastra(window);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            g1.aStar(window);
        }

        window.clear(sf::Color(240, 240, 240)); // Light grey background, close to white
        g1.displayGrid(window);

        window.display();
    }

    return EXIT_SUCCESS;
}
