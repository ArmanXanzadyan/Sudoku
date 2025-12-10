#ifndef GUIBOARD_HPP
#define GUIBOARD_HPP

#include <QWidget>
#include <QFont>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include "../headers/Sudoku.hpp"

class GuiBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GuiBoard(QWidget* parent = nullptr);

    QSize sizeHint() const override { return QSize(450, 450); }
    void setSudoku(Sudoku* s) { sudoku_ = s; update(); }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Sudoku* sudoku_;
    int selectedRow_;
    int selectedCol_;
    QFont font_;
};

#endif // GUIBOARD_HPP

