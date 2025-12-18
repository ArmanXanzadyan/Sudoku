#include "GuiBoard.hpp"
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QDebug>

GuiBoard::GuiBoard(QWidget* parent)
    : QWidget(parent), sudoku_(nullptr), selectedRow_(-1), selectedCol_(-1)
{
    font_ = QFont("DejaVu Sans", 20);
}

void
GuiBoard::paintEvent(QPaintEvent*)
{
    if (!sudoku_) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width() / SIZE;
    int h = height() / SIZE;

    for (int i = 0; i <= SIZE; ++i) {
        painter.setPen(i % BLOCKS_IN_ROW == 0 ? QPen(Qt::black, 2) : QPen(Qt::black, 1));
        painter.drawLine(0, i*h, width(), i*h);
        painter.drawLine(i*w, 0, i*w, height());
    }

    painter.setFont(font_);
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            int val = sudoku_->grid_[r][c].first;
            if (val == 0) continue;

            QRect rect(c*w, r*h, w, h);
            if (!sudoku_->grid_[r][c].second)
                painter.setPen(Qt::red);
            else
                painter.setPen(Qt::black);

            painter.drawText(rect, Qt::AlignCenter, QString::number(val));
        }
    }

    if (selectedRow_ >= 0 && selectedCol_ >= 0) {
        painter.setBrush(QColor(0, 0, 255, 50));
        painter.drawRect(selectedCol_*w, selectedRow_*h, w, h);
    }
}

void
GuiBoard::mousePressEvent(QMouseEvent* event)
{
    int w = width() / 9;
    int h = height() / 9;

    QPointF pos = event->position();
    selectedCol_ = static_cast<int>(pos.x()) / w;
    selectedRow_ = static_cast<int>(pos.y()) / h;

    update();
}

void
GuiBoard::keyPressEvent(QKeyEvent* event)
{
    if (!sudoku_ || selectedRow_ < 0 || selectedCol_ < 0) return;

    int key = event->key();
    if (key >= Qt::Key_1 && key <= Qt::Key_9) {
        int val = key - Qt::Key_0;
        if (sudoku_->isValid(selectedRow_, selectedCol_, val)) {
            sudoku_->setCellValue(selectedRow_, selectedCol_, val);
            update();
        }
    }
}

