#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <cstddef>
#include <utility>
#include <vector>

const int SIZE = 9;
const int BLOCKS_IN_ROW = 3;

class Sudoku {
public:
    Sudoku();
    void print() const;
    bool isValid(const size_t row, const size_t col, const int num) const;
    int getCellValue(const size_t row, const size_t col) const;
    void setCellValue(const size_t row, const size_t col, const int val);

    int getGridNumber(const int row, const int column) const;
    bool getGridFilled(const int row, const int column) const;
    bool isSuccesfulEnd();
    void play();
    void solveSudoko();
    void generateRandomSudoku(const int percent);
    std::vector<int> getPossibleValues(const size_t row, const size_t col) const;
private:
    bool fillGrid(const size_t row, const size_t col);
    int getRandomNumber() const;
    void init();
    bool isHorizontal(const size_t row, const int value) const;
    bool isVertical(const size_t col, const int value) const;
    bool isBlock(const size_t row, const size_t col, const int value) const;
    void printRow() const;
    bool isFull() const;
    int getNumber() const;
    std::pair<int, int> getPath() const;
    bool isValidSudoko();
    bool canWePutElement(const int row, const int column) const;
private:
    std::pair<int, bool> grid_[SIZE][SIZE];
};

#endif // SUDOKU_HPP

