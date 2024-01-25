#include <SFML/Graphics.hpp>
#include<math.h>
#include <windows.h>
#include <iostream>
#include <cstring>
#include <stack>
#include <queue>
using namespace std;

#include "./includes/fonts.cpp"
UseFonts colus("colus.otf", "fonts/colus.otf");


class cell;

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


    cell * endcell=nullptr;
    cell * startcell=nullptr;


    string window_title="Path Finding Algorithm";


    bool block_inserting = true;
    bool solve=false;
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

window_settings Window(18,32);

class cell{
    public:
    int col_pos;   //j
    int row_pos;   //i
    int x_cod;
    int y_cod;

    bool isBlocked=false;
    bool isEnd=false;
    bool isStart=false;
    bool isunderCursor=false;
    bool isSoultion=false;
    bool isTraversed=false;
    bool isNeighbour=false;

    sf::Color cellColor;


    cell * parent = nullptr;
    float gCost=0;
    float hCost=0;
    float fcost=0;

    
    float h_value(){
        cell end = *Window.endcell;
        float dx = abs(col_pos - end.col_pos);
        float dy = abs(row_pos - end.row_pos);
        hCost=(dx+dy);
        return hCost;
    }


    void calculate_cost(){
        h_value();
        fcost=gCost+hCost;
    }

    void display_cost(){
        cout<<"H: "<<hCost<<" G: "<<gCost<<" F: "<<fcost<<endl;
    }

    cell(int a=12, int b=5){

        col_pos = a;
        row_pos = b;

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



     bool operator==(const cell& other) const {
        // Define your equality logic based on the class properties
        return ((x_cod == other.x_cod)&&(y_cod == other.y_cod));
    }

    void update(){
        if(isBlocked){
            cellColor = Window.blockade;
        }else if( isStart){
            cellColor = sf::Color::Green;
        }else if( isEnd){
            cellColor = sf::Color::Magenta;
        }else if(isSoultion){
            cellColor = sf::Color::Magenta;
        }else if(isunderCursor){
            cellColor = Window.mouse;
        }else if(isTraversed){
            cellColor = Window.iteration;
        }else if(isNeighbour){
            cellColor = sf::Color::Blue;
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


      bool operator<(const cell& other) const {
        // Priority queue should be in ascending order based on fcost
        return fcost > other.fcost;
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
                        if(grid[i][j].isStart==false && grid[i][j].isEnd==false){
                            grid[i][j].isBlocked=true;

                        }
                        
                    }else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && Window.erasing==true){
                        grid[i][j].isBlocked=false;
                    }else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && Window.start==true){
                        grid[i][j].isStart=true;
                        Window.startcell=&grid[i][j];
                        Window.start=false;
                    }else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && Window.end==true){
                        grid[i][j].isEnd=true;
                        Window.endcell=&grid[i][j];
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
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T )
        {
            Window.block_inserting = false;
            Window.erasing = true;
            Window.end = false;
            Window.start = false;
        }

         if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q )
        {
            Window.block_inserting = false;
            Window.erasing = false;
            Window.solve = true;
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



void tracepath(){
    int row = Window.endcell->row_pos;
    int col = Window.endcell->col_pos;
    
        cout<<"outside_while"<<endl;
    while(!((grid.grid[row][col].parent->row_pos == row && grid.grid[row][col].parent->col_pos == col))){
        cout<<"inside_while"<<endl;
        grid.grid[row][col].isSoultion=true;
        int temp_row = grid.grid[row][col].parent->row_pos;
        int temp_col = grid.grid[row][col].parent->col_pos;
        row=temp_row;
        col=temp_col;
    }

     cout<<"completed_while"<<endl;

}

bool checktarget(cell current){
   
   
    if(current.row_pos==Window.endcell->row_pos && current.col_pos==Window.endcell->col_pos){
        cout<<"hw_end"<<endl;
        return true;
    }
    return false;
}


bool isCellInPriorityQueue(const std::priority_queue<cell>&pq, const cell& target) {
    std::priority_queue<cell> temp = pq;  
    // Iterate through the copy to find the target
    while (!temp.empty()) {
        if (temp.top().col_pos == target.col_pos && temp.top().row_pos == target.row_pos) {
            return true;  // Found the target
        }
        temp.pop();
    }

    return false;  // Target not found
}


void Astar(){
    cell start = *Window.startcell;
    cell end = *Window.endcell;

   

    std::priority_queue<cell> openList;
    std::priority_queue<cell> closedList;

    start.fcost=0;
    start.parent=&start;
    openList.push(start);


    cout<<"Start Test"<<endl;

;
    while(!openList.empty()){
        static int i=1150;
        cell current = openList.top();
        cout<<endl<<endl;
        cout<<"current selected: "<<current.row_pos<<" "<<current.col_pos<<" | All nighbours "<<endl;

        openList.pop();
        closedList.push(current);

        if(current.isEnd==true){
            cout<<"traching_target"<<endl;
            tracepath();
            cout<<"found_end"<<endl;
            return;
        }
        if(i<=0){
            break;
        }
        i--;

      
    
       

        for(int i=-1;i<=1;i++){
            for(int j=-1;j<=1;j++){
                 
                
                if((i+j==0)||(abs(i+j)>=2)){
                    continue;
                }

                if(current.row_pos+i>=0 && current.row_pos+i<Window.rows && current.col_pos+j>=0 && current.col_pos+j<Window.columns){


                    cell * neighbour = &grid.grid[current.row_pos+i][current.col_pos+j];
                    
                    cout<<endl<<"neighbour:"<<neighbour->row_pos<<" "<<neighbour->col_pos<<"  ";
                    
                    if((neighbour->isBlocked ) ){
                        neighbour->isNeighbour=true;
                       
                        cout<<"  is_blocked: "<<neighbour->isBlocked;
                        cout<<"  is_end: "<<neighbour->isEnd;
                        cout<<"  neighbour blocked / skipped: "<<neighbour->row_pos<<" "<<neighbour->col_pos;

                        continue;
                    }

                    cout<<" cost: ";
                    cout<<"case: "<<( current.gCost + 1 <= neighbour->gCost)<<" "<<!isCellInPriorityQueue(openList, *neighbour);


                    if(( current.gCost + 1 <= neighbour->gCost) || !isCellInPriorityQueue(openList, *neighbour)){ 
                       
                        neighbour->gCost = current.gCost+1;
                        neighbour->calculate_cost();
                        neighbour->display_cost();
                        neighbour->parent = &current;

                        
                        if(!isCellInPriorityQueue(openList, *neighbour)){
                            openList.push(*neighbour);
                            neighbour->isTraversed=true;
                        }

                    }

                    

                }
            }
        }


    }
}





int main(){
    sf::RenderWindow window;
    window.create(sf::VideoMode(1280, 720), Window.window_title, sf::Style::Titlebar | sf::Style::Close);
    
      while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Create a text object
        sf::Text text, text2;
        text.setString(std::to_string(mousePos.y / static_cast<int>(Window.cellHeight)));
        text2.setString(std::to_string(mousePos.x / static_cast<int>(Window.cellWidth)));
        text.setFont(colus.font);
        text2.setFont(colus.font);
        text.setCharacterSize(24);
        text2.setCharacterSize(24); // in pixels, not points!
        text.setFillColor(sf::Color::Magenta);
        text2.setFillColor(sf::Color::Magenta);
        text.setPosition(50, 50);
        text2.setPosition(50, 100);
    

        handle_events(window);         
        window.clear(sf::Color(240, 240, 240));
        window.draw(text);
        window.draw(text2);
        
        if(Window.solve==true){
            Astar(); 
            cout<<"here"<<endl;
            Window.solve=false;
        }
        grid.update(window);
        
        window.display();
    }



    return 0;
}