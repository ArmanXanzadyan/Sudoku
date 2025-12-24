#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Sudoku");
    setFixedSize(520, 650);

    QLabel* levelLabel = new QLabel("Select Level:");

    levelBox_ = new QComboBox;
    levelBox_->addItem("Very Easy (10%)", 10);
    levelBox_->addItem("Easy (25%)", 25);
    levelBox_->addItem("Normal (40%)", 40);
    levelBox_->addItem("Medium (50%)", 50);
    levelBox_->addItem("Hard (70%)", 70);
    levelBox_->addItem("Very Hard (80%)", 80);
    levelBox_->addItem("Extreme (90%)", 90);

    QHBoxLayout* levelLayout = new QHBoxLayout;
    levelLayout->addWidget(levelLabel);
    levelLayout->addWidget(levelBox_);

    QLabel* title = new QLabel("SUDOKU");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:28px; font-weight:bold;");

    board_ = new GuiBoard(this);
    sudoku_.generateRandomSudoku(50);
    board_->setSudoku(&sudoku_);

    QPushButton* newBtn   = new QPushButton("New Game");
    QPushButton* clearBtn = new QPushButton("Clear");
    QPushButton* checkBtn = new QPushButton("Check");
    QPushButton* hintBtn  = new QPushButton("Hints");
    QPushButton* exitBtn  = new QPushButton("Exit");

    QString style =
        "QPushButton { font-size:16px; padding:8px; border-radius:8px; "
        "background:#3498db; color:white; }"
        "QPushButton:hover { background:#2980b9; }";

    newBtn->setStyleSheet(style);
    clearBtn->setStyleSheet(style);
    checkBtn->setStyleSheet(style);
    hintBtn->setStyleSheet(style);
    exitBtn->setStyleSheet(
        "QPushButton { font-size:16px; padding:8px; border-radius:8px; "
        "background:#e74c3c; color:white; }"
    );

    connect(newBtn,   &QPushButton::clicked, this, &MainWindow::newGame);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearBoard);
    connect(checkBtn, &QPushButton::clicked, this, &MainWindow::checkWin);
    connect(hintBtn,  &QPushButton::clicked, board_, &GuiBoard::toggleHints);
    connect(exitBtn,  &QPushButton::clicked, this, &QWidget::close);

    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addWidget(newBtn);
    buttons->addWidget(clearBtn);
    buttons->addWidget(checkBtn);
    buttons->addWidget(hintBtn);
    buttons->addWidget(exitBtn);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(levelLayout);
    layout->addWidget(title);
    layout->addWidget(board_);
    layout->addLayout(buttons);
}

void
MainWindow::newGame()
{
    int percent = levelBox_->currentData().toInt();
    sudoku_.generateRandomSudoku(percent);
    board_->update();
}

void
MainWindow::clearBoard()
{
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (sudoku_.getGridFilled(r, c))
                sudoku_.setCellValue(r, c, 0);

    board_->update();
}

void
MainWindow::checkWin()
{
    if (sudoku_.isSuccesfulEnd()) {
        QMessageBox::information(
            this, "Sudoku",
            "🎉 Congratulations! You solved the Sudoku! 🎉"
        );
    } else {
        QMessageBox::warning(
            this, "Sudoku",
            "❌ Sudoku is not complete or has errors!"
        );
    }
}

