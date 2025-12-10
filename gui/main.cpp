#include <QApplication>
#include "GuiBoard.hpp"
#include "../headers/Sudoku.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Sudoku sudoku;
    sudoku.generateRandomSudokuColored(50);

    GuiBoard board;
    board.setSudoku(&sudoku);
    board.show();

    return app.exec();
}

