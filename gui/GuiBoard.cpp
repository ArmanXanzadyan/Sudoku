#include "GuiBoard.hpp"

GuiBoard::GuiBoard(QWidget* parent)
    : QWidget(parent),
      sudoku_(nullptr),
      row_(-1),
      col_(-1),
      showHints_(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(450, 450);
}

void
GuiBoard::setSudoku(Sudoku* s)
{
    sudoku_ = s;
    update();
}

void
GuiBoard::toggleHints()
{
    showHints_ = !showHints_;
    possible_.clear();
    update();
}

void
GuiBoard::mousePressEvent(QMouseEvent* e)
{
    col_ = e->position().x() / (width() / SIZE);
    row_ = e->position().y() / (height() / SIZE);

    possible_.clear();

    if (showHints_ &&
        sudoku_ &&
        sudoku_->getGridFilled(row_, col_) &&
        sudoku_->getGridNumber(row_, col_) == 0)
    {
        possible_ = sudoku_->getPossibleValues(row_, col_);
    }

    setFocus();
    update();
}

void
GuiBoard::paintEvent(QPaintEvent*)
{
    if (!sudoku_) return;

    QPainter p(this);
    const int w = width() / SIZE;
    const int h = height() / SIZE;

    for (int i = 0; i <= SIZE; ++i) {
        p.setPen(QPen(Qt::black, i % 3 == 0 ? 2 : 1));
        p.drawLine(0, i*h, width(), i*h);
        p.drawLine(i*w, 0, i*w, height());
    }

    if (row_ >= 0 && col_ >= 0)
        p.fillRect(col_*w, row_*h, w, h, QColor(0,120,215,80));
    QFont f;
    f.setPointSize(h * 0.45);
    p.setFont(f);

    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            const int v = sudoku_->getGridNumber(r, c);
            if (v == 0) continue;

            p.setPen(sudoku_->getGridFilled(r, c) ? Qt::black : Qt::red);
            p.drawText(c*w, r*h, w, h, Qt::AlignCenter, QString::number(v));
        }
    }
    if (showHints_ &&
        row_ >= 0 && col_ >= 0 &&
        sudoku_->getGridFilled(row_, col_) &&
        sudoku_->getGridNumber(row_, col_) == 0)
    {
        QFont small = p.font();
        small.setPointSize(h * 0.18);
        p.setFont(small);
        p.setPen(Qt::gray);

        int idx = 0;
        for (int num : possible_) {
            int x = col_*w + (idx % 3) * (w/3);
            int y = row_*h + (idx / 3) * (h/3);
            p.drawText(x, y, w/3, h/3, Qt::AlignCenter, QString::number(num));
            ++idx;
        }
    }
}

void
GuiBoard::keyPressEvent(QKeyEvent* e)
{
    if (!sudoku_ || row_ < 0 || col_ < 0) return;
    if (!sudoku_->getGridFilled(row_, col_)) return;

    int key = e->key();
    if (key >= Qt::Key_1 && key <= Qt::Key_9) {
        if (sudoku_->getGridNumber(row_, col_) == 0) {
            sudoku_->setCellValue(row_, col_, key - Qt::Key_0);
            update();
        }
    }

    if (key == Qt::Key_Backspace) {
        sudoku_->setCellValue(row_, col_, 0);
        update();
    }
}

