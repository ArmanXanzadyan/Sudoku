QT += widgets
CONFIG += c++17

# Path to headers
INCLUDEPATH += ../headers

# GUI sources
SOURCES += main.cpp \
           GuiBoard.cpp \
           ../sources/Sudoku.cpp   # <-- add core cpp here

HEADERS += GuiBoard.hpp \
           ../headers/Sudoku.hpp

