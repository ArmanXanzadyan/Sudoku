#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>

#include "GuiBoard.hpp"
#include "../headers/Sudoku.hpp"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void newGame();
    void clearBoard();
    void checkWin();

private:
    Sudoku sudoku_;
    GuiBoard* board_;
    QComboBox* levelBox_;
};

#endif

