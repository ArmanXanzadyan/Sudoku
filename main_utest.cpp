#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include "headers/Sudoku.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem> // C++17 required for path checks

TEST(SudokuTest, PlayGUI)
{
    Sudoku sudoku;
    sudoku.generateRandomSudoku(50);
    sudoku.play();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

