#include "headers/Sudoku.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

Sudoku::Sudoku()
{
    std::srand(std::time(0));
    init();
}

void
Sudoku::init()
{
    for (size_t i = 0; i < SIZE; ++i)
        for (size_t j = 0; j < SIZE; ++j)
            grid_[i][j] = {0, false};
}

int
Sudoku::getRandomNumber() const
{
    return std::rand() % SIZE;
}

bool
Sudoku::isHorizontal(const size_t row, const int value) const
{
    for (size_t i = 0; i < SIZE; ++i)
        if (grid_[row][i].first == value) return false;
    return true;
}

bool
Sudoku::isVertical(const size_t col, const int value) const
{
    for (size_t i = 0; i < SIZE; ++i)
        if (grid_[i][col].first == value) return false;
    return true;
}

bool
Sudoku::isBlock(const size_t row, const size_t col, const int value) const
{
    size_t startRow = (row / BLOCKS_IN_ROW) * BLOCKS_IN_ROW;
    size_t startCol = (col / BLOCKS_IN_ROW) * BLOCKS_IN_ROW;
    for (size_t i = startRow; i < startRow + BLOCKS_IN_ROW; ++i)
        for (size_t j = startCol; j < startCol + BLOCKS_IN_ROW; ++j)
            if (grid_[i][j].first == value) return false;
    return true;
}

bool
Sudoku::isValid(const size_t row, const size_t col, const int number) const
{
    return isHorizontal(row, number) && isVertical(col, number) && isBlock(row, col, number);
}

bool
Sudoku::fillGrid(const size_t row, const size_t col)
{
    if (row == SIZE) return true;
    size_t nextRow = (col == SIZE - 1) ? row + 1 : row;
    size_t nextCol = (col == SIZE - 1) ? 0 : col + 1;
    if (grid_[row][col].first != 0) return fillGrid(nextRow, nextCol);

    int numbers[SIZE];
    for (int i = 0; i < SIZE; ++i) numbers[i] = i + 1;
    for (int i = 0; i < SIZE; ++i) std::swap(numbers[i], numbers[std::rand() % SIZE]);

    for (int i = 0; i < SIZE; ++i) {
        int num = numbers[i];
        if (isValid(row, col, num)) {
            grid_[row][col].first = num;
            if (fillGrid(nextRow, nextCol)) return true;
            grid_[row][col].first = 0;
        }
    }
    return false;
}

void
Sudoku::generateRandomSudokuColored(const int percent)
{
    init();
    fillGrid(0, 0);

    size_t removeCount = SIZE * SIZE * percent / 100;
    for (size_t i = 0; i < removeCount;) {
        int row = getRandomNumber();
        int col = getRandomNumber();
        if (!grid_[row][col].second) {
            grid_[row][col].first = 0;
            grid_[row][col].second = true; // սև՝ user-fillable
            ++i;
        }
    }

    for (size_t i = 0; i < SIZE; ++i)
        for (size_t j = 0; j < SIZE; ++j)
            if (!grid_[i][j].second && grid_[i][j].first != 0)
                grid_[i][j].second = false; // կարմիր՝ pre-filled
}

void
Sudoku::setCellValue(const size_t row, const size_t col, const int val)
{
    grid_[row][col].first = val;
}

bool
Sudoku::isFull() const
{
    for (size_t i = 0; i < SIZE; ++i)
        for (size_t j = 0; j < SIZE; ++j)
            if (grid_[i][j].first == 0) return false;
    return true;
}

bool
Sudoku::isValidSudoko()
{
    for (size_t row = 0; row < SIZE; ++row) {
        for (size_t col = 0; col < SIZE; ++col) {
            int num = grid_[row][col].first;
            if (num == 0) continue;

            grid_[row][col].first = 0;          // ժամանակավոր հանում ենք
            bool valid = isValid(row, col, num);
            grid_[row][col].first = num;        // հետ ենք դնում

            if (!valid) return false;
        }
    }
    return true;
}

void Sudoku::play()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Milena");
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) return;

    const int cellSize = 600 / SIZE;
    sf::RectangleShape cell(sf::Vector2f(cellSize - 2.f, cellSize - 2.f));
    cell.setOutlineColor(sf::Color::Black);
    cell.setOutlineThickness(1.f);

    int selectedRow = -1, selectedCol = -1;
    bool gameWon = false;
    sf::Clock winClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (!gameWon) { // եթե դեռ չի հաղթել, թույլ ենք տալիս մուտքագրումներ
                if (event.type == sf::Event::MouseButtonPressed) {
                    selectedCol = event.mouseButton.x / cellSize;
                    selectedRow = event.mouseButton.y / cellSize;
                }
                if (event.type == sf::Event::TextEntered && selectedRow >= 0 && selectedCol >= 0) {
                    if (event.text.unicode >= '1' && event.text.unicode <= '9') {
                        int number = event.text.unicode - '0';
                        if (isValid(selectedRow, selectedCol, number))
                            setCellValue(selectedRow, selectedCol, number);
                    }
                }
            }
        }

        window.clear(sf::Color(200, 200, 200));

        // --- Նկարում ենք Sudoku վանդակները ---
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                cell.setPosition(j * cellSize + 1.f, i * cellSize + 1.f);
                if (grid_[i][j].first == 0)
                    cell.setFillColor(sf::Color(245, 245, 245));
                else if (grid_[i][j].second)
                    cell.setFillColor(sf::Color::Black);
                else
                    cell.setFillColor(sf::Color(180, 0, 0));

                if (i == selectedRow && j == selectedCol)
                    cell.setFillColor(sf::Color(200, 200, 255));

                window.draw(cell);

                int val = grid_[i][j].first;
                if (val != 0) {
                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(val));
                    text.setCharacterSize(40);
                    text.setFillColor(sf::Color::White);
                    text.setPosition(j * cellSize + 18.f, i * cellSize + 5.f);
                    window.draw(text);
                }
            }
        }

        // --- Խիտ գծեր 3x3 բլոկների համար ---
        for (int i = 0; i <= SIZE; ++i) {
            int thick = (i % 3 == 0) ? 3 : 1;
            sf::RectangleShape h(sf::Vector2f(600, thick));
            h.setPosition(0, i * cellSize);
            h.setFillColor(sf::Color::Black);
            window.draw(h);

            sf::RectangleShape v(sf::Vector2f(thick, 600));
            v.setPosition(i * cellSize, 0);
            v.setFillColor(sf::Color::Black);
            window.draw(v);
        }

        // --- Եթե հաղթել է ---
        if (!gameWon && isFull() && isValidSudoko()) {
            gameWon = true;
            winClock.restart();
        }

        if (gameWon) {
            // Պատուհանի վերևում փափուկ մթնեցում
            sf::RectangleShape overlay(sf::Vector2f(600, 600));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            // "Bravo you win!" գրվածքը
            sf::Text winText;
            winText.setFont(font);
            winText.setString("🎉 Bravo, You Win! 🎉");
            winText.setCharacterSize(50);
            winText.setFillColor(sf::Color::Yellow);

            // Կենտրոնում տեղադրում
            sf::FloatRect textRect = winText.getLocalBounds();
            winText.setOrigin(textRect.width / 2, textRect.height / 2);
            winText.setPosition(300, 280);

            window.draw(winText);

            // 3 վայրկյան անց փակենք պատուհանը
            if (winClock.getElapsedTime().asSeconds() > 3.f)
                window.close();
        }

        window.display();
    }
}
/*
void Sudoku::play()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Milena Sudoku");
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) return;

    const int cellSize = 600 / SIZE;
    sf::RectangleShape cell(sf::Vector2f(cellSize - 2.f, cellSize - 2.f));
    cell.setOutlineColor(sf::Color::Black);
    cell.setOutlineThickness(1.f);

    int selectedRow = -1, selectedCol = -1;
    bool hasWon = false; // ✅ նոր փոփոխական՝ հաղթանակի վիճակը պահելու համար

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!hasWon) { // ✅ եթե հաղթած է՝ այլևս չթույլատրենք մուտք
                if (event.type == sf::Event::MouseButtonPressed) {
                    selectedCol = event.mouseButton.x / cellSize;
                    selectedRow = event.mouseButton.y / cellSize;
                }
                if (event.type == sf::Event::TextEntered && selectedRow >= 0 && selectedCol >= 0) {
                    if (event.text.unicode >= '1' && event.text.unicode <= '9') {
                        int number = event.text.unicode - '0';
                        if (isValid(selectedRow, selectedCol, number))
                            setCellValue(selectedRow, selectedCol, number);
                    }
                }
            }
        }

        window.clear(sf::Color(220, 220, 220));

        // === վանդակի գծում ===
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                cell.setPosition(j * cellSize + 1.f, i * cellSize + 1.f);
                if (grid_[i][j].first == 0)
                    cell.setFillColor(sf::Color(245, 245, 245));
                else if (grid_[i][j].second)
                    cell.setFillColor(sf::Color::Black);
                else
                    cell.setFillColor(sf::Color(200, 0, 0));

                if (i == selectedRow && j == selectedCol)
                    cell.setFillColor(sf::Color(180, 180, 255));

                window.draw(cell);

                int val = grid_[i][j].first;
                if (val != 0) {
                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(val));
                    text.setCharacterSize(40);
                    text.setFillColor(sf::Color::White);
                    text.setPosition(j * cellSize + 18.f, i * cellSize + 5.f);
                    window.draw(text);
                }
            }
        }

        // === ցանցի հաստ գծեր ===
        for (int i = 0; i <= SIZE; ++i) {
            int thick = (i % 3 == 0) ? 3 : 1;
            sf::RectangleShape h(sf::Vector2f(600, thick));
            h.setPosition(0, i * cellSize);
            h.setFillColor(sf::Color::Black);
            window.draw(h);

            sf::RectangleShape v(sf::Vector2f(thick, 600));
            v.setPosition(i * cellSize, 0);
            v.setFillColor(sf::Color::Black);
            window.draw(v);
        }

        // === ստուգում ենք հաղթանակը ===
        if (!hasWon && isFull() && isValidSudoko()) {
            hasWon = true;
        }

        // === եթե հաղթել է՝ հաղորդագրություն ցույց տալ ===
        if (hasWon) {
            sf::Text winText;
            winText.setFont(font);
            winText.setString("🎉 YOU WIN! 🎉");
            winText.setCharacterSize(70);
            winText.setFillColor(sf::Color(50, 205, 50)); // պայծառ կանաչ
            winText.setStyle(sf::Text::Bold);
            winText.setPosition(90.f, 250.f);
            window.draw(winText);
        }

        window.display();
    }
}
*/
