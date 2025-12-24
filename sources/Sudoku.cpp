#include "../headers/Sudoku.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>

Sudoku::Sudoku()
{
    std::srand(std::time(0));
    init();
}

void
Sudoku::init()
{
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            grid_[i][j] = {0, false};
        }
    }
}

int
Sudoku::getRandomNumber() const
{
    return std::rand() % SIZE;
}


/////////////////////////////////// CHECK FUNCTIONS


bool
Sudoku::isHorizontal(const size_t row, const int value) const
{
    for (size_t i = 0; i < SIZE; ++i) {
        if (grid_[row][i].first == value) return false;
    }
    return true;
}

bool
Sudoku::isVertical(const size_t col, const int value) const
{
    for (size_t i = 0; i < SIZE; ++i) {
        if (grid_[i][col].first == value) return false;
    }
    return true;
}

bool
Sudoku::isBlock(const size_t row, const size_t col, const int value) const
{
    const size_t startRow = (row / BLOCKS_IN_ROW) * BLOCKS_IN_ROW;
    const size_t startCol = (col / BLOCKS_IN_ROW) * BLOCKS_IN_ROW;
    for (size_t i = startRow; i < startRow + BLOCKS_IN_ROW; ++i) {
        for (size_t j = startCol; j < startCol + BLOCKS_IN_ROW; ++j) {
            if (grid_[i][j].first == value) return false;
        }
    }
    return true;
}

bool
Sudoku::isValid(const size_t row, const size_t col, const int number) const
{
    return isHorizontal(row, number) && isVertical(col, number) && isBlock(row, col, number);
}


////////////////////////////////////////////////////////////////////






bool
Sudoku::fillGrid(const size_t row, const size_t col)
{
    if (row == SIZE) return true;
    const size_t nextRow = (col == SIZE - 1) ? row + 1 : row;
    const size_t nextCol = (col == SIZE - 1) ? 0 : col + 1;
    if (grid_[row][col].first != 0) return fillGrid(nextRow, nextCol);

    int numbers[SIZE];
    for (int i = 0; i < SIZE; ++i) numbers[i] = i + 1;
    for (int i = 0; i < SIZE; ++i) std::swap(numbers[i], numbers[std::rand() % SIZE]);

    for (int i = 0; i < SIZE; ++i) {
        const int num = numbers[i];
        if (isValid(row, col, num)) {
            grid_[row][col].first = num;
            if (fillGrid(nextRow, nextCol)) return true;
            grid_[row][col].first = 0;
        }
    }
    return false;
}


void
Sudoku::generateRandomSudoku(const int percent)
{
    init();
    fillGrid(0, 0);

    const size_t removeCount = SIZE * SIZE * percent / 100;

    size_t removed = 0;
    while (removed < removeCount) {
        const int row = getRandomNumber();
        const int col = getRandomNumber();

        if (grid_[row][col].first != 0) {
            grid_[row][col].first = 0;
            ++removed;
        }
    }

    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            grid_[i][j].second = (0 == grid_[i][j].first);
        }
    }
}


void
Sudoku::setCellValue(const size_t row, const size_t col, const int val)
{
    grid_[row][col].first = val;
}

bool
Sudoku::isFull() const
{
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            if (grid_[i][j].first == 0) return false;
        }
    }
    return true;
}

bool
Sudoku::isValidSudoko()
{
    for (size_t row = 0; row < SIZE; ++row) {
        for (size_t col = 0; col < SIZE; ++col) {
            const int num = grid_[row][col].first;
            if (num == 0) continue;

            grid_[row][col].first = 0;
            const bool valid = isValid(row, col, num);
            grid_[row][col].first = num;

            if (!valid) return false;
        }
    }
    return true;
}

void
Sudoku::print() const
{
    const size_t blockSize = static_cast<size_t>(std::sqrt(SIZE));

    std::cout << "\nSudoku:\n\n";
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            const int v = grid_[i][j].first;
            if (v == 0) std::cout << ". ";
            else std::cout << v << " ";

            if ((j + 1) % blockSize == 0 && j != SIZE - 1) std::cout << "| ";
        }
        std::cout << "\n";
        if ((i + 1) % blockSize == 0 && i != SIZE - 1) {
            for (size_t k = 0; k < SIZE + blockSize - 1; ++k) std::cout << "- ";
            std::cout << "\n";
        }
    }
}

int
Sudoku::getNumber() const
{
    int x;
    std::cin >> x;
    return x;
}

bool
Sudoku::isSuccesfulEnd()
{
    return isFull() && isValidSudoko();
}


bool
Sudoku::canWePutElement(const int row, const int column) const
{
    return grid_[row][column].second;
}

void
Sudoku::play()
{
    while (true) {
        print();

        if (isSuccesfulEnd()) {
            std::cout << "\n🎉 Bravo! You solved the Sudoku! 🎉\n";
            return;
        }

        std::cout << "\nInput row col value (example: 3 4 9)\n";

        const int r = getNumber();
        const int c = getNumber();
        const int val = getNumber();
        const size_t row = static_cast<size_t>(r - 1);
        const size_t col = static_cast<size_t>(c - 1);
        if (!canWePutElement(row, col)) {
            std::cout << "❌ You can't change this cell!\n";
            continue;
        }
        if (!isValid(row, col, val)) {
            std::cout << "❌ Invalid value!\n";
            continue;
        }
        setCellValue(row, col, val);
    }
}


std::vector<int>
Sudoku::getPossibleValues(const size_t row, const size_t col) const
{
    std::vector<int> res;
    if (!grid_[row][col].second || grid_[row][col].first != 0)
        return res;
    for (int num = 1; num <= 9; ++num) {
        if (isValid(row, col, num)) {
            res.push_back(num);
        }
    }
    return res;
}

int
Sudoku::getGridNumber(const int row, const int column) const
{
    return grid_[row][column].first;
}

bool
Sudoku::getGridFilled(const int row, const int column) const
{
    return grid_[row][column].second;
}

