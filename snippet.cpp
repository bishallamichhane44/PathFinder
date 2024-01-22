

// Code to color the cell green under the mouse cursor and black for cells selected with click and drag
sf::Vector2i mousePos = sf::Mouse::getPosition(window);
int rowUnderMouse = mousePos.y / static_cast<int>(cellHeight);
int columnUnderMouse = mousePos.x / static_cast<int>(cellWidth);

bool mouseHeld = sf::Mouse::isButtonPressed(sf::Mouse::Left);
static bool dragging = false;
static std::vector<sf::Vector2i> selectedCells;

// Start dragging
if (mouseHeld && !dragging) {
    dragging = true;
    selectedCells.clear();
}

// While dragging, add cells to the selection
if (dragging && mouseHeld) {
    selectedCells.push_back({columnUnderMouse, rowUnderMouse});
}

// Stop dragging
if (!mouseHeld && dragging) {
    dragging = false;
}

// Draw cells
for (int row = 0; row < rows; ++row) {
    for (int column = 0; column < columns; ++column) {
        // Check if the current cell is being dragged over
        bool cellSelected = std::find(selectedCells.begin(), selectedCells.end(), sf::Vector2i(column, row)) != selectedCells.end();
        
        if (row == rowUnderMouse && column == columnUnderMouse) {
            cell.setFillColor(sf::Color::Green); // Green color for cell under mouse
        } else if (cellSelected) {
            cell.setFillColor(sf::Color::Black); // Black color for selected cells
        } else {
            cell.setFillColor(sf::Color::Transparent);
        }
        
        cell.setPosition(column * cellWidth, row * cellHeight);
        window.draw(cell);
    }
}