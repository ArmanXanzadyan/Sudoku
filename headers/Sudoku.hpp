#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <cstddef>
#include <utility>

const int SIZE = 9;
const int BLOCKS_IN_ROW = 3;

class Sudoku {
public:
    Sudoku();
    void print() const;
    bool isValid(const size_t row, const size_t col, const int num) const;
    int getCellValue(const size_t row, const size_t col) const;
    void setCellValue(const size_t row, const size_t col, const int val);

    void play();
    void solveSudoko();
    void generateRandomSudokuColored(const int percent);

private:
    bool fillGrid(const size_t row, const size_t col);
    int getRandomNumber() const;
    void init();
    int getRightNumber(const size_t row, const size_t col) const;
    bool isHorizontal(const size_t row, const int value) const;
    bool isVertical(const size_t col, const int value) const;
    bool isBlock(const size_t row, const size_t col, const int value) const;
    void printRow() const;
    bool isFull() const;
    int getNumber() const;
    std::pair<int, int> getPath() const;
    bool isValidSudoko();

private:
    std::pair<int, bool> grid_[SIZE][SIZE]; // first = value, second = true->user-fillable(սև), false->pre-filled(կարմիր)
};

#endif // SUDOKU_HPP

