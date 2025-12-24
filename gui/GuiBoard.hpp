#ifndef GUIBOARD_HPP
#define GUIBOARD_HPP

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <vector>
#include "../headers/Sudoku.hpp"

class GuiBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GuiBoard(QWidget* parent = nullptr);
    void setSudoku(Sudoku* s);
    void toggleHints();

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

private:
    Sudoku* sudoku_;
    int row_;
    int col_;
    bool showHints_;
    std::vector<int> possible_;
};

#endif

