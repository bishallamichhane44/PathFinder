 // Calculate the difference in cells between the currently selected cell
    // and the last selected cell
    int deltaRow = abs(rowUnderMouse - lastRow);
    int deltaCol = abs(columnUnderMouse - lastCol);

    // Determine the direction of selection for rows and columns
    int rowStep = (rowUnderMouse > lastRow) ? 1 : -1;
    int colStep = (columnUnderMouse > lastCol) ? 1 : -1;

    // Fill in the gaps to include cells missed due to fast mouse movement
    for (int i = 0; i <= deltaRow; ++i) {
        for (int j = 0; j <= deltaCol; ++j) {
            int intermediateRow = lastRow + i * rowStep;
            int intermediateCol = lastCol + j * colStep;
            sf::Vector2i intermediateCell(intermediateCol, intermediateRow);
            // Add the intermediate cell if it is not already in the selection
            if (std::find(selectedCells.begin(), selectedCells.end(), intermediateCell) == selectedCells.end()) {
                selectedCells.push_back(intermediateCell);
            }
        }
    }

    // Update the last row and column to the current row and column
    lastRow = rowUnderMouse;
    lastCol = columnUnderMouse;


    