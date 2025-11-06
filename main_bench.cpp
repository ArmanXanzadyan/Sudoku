#include <benchmark/benchmark.h>
#include <SFML/Graphics.hpp>
#include "headers/Sudoku.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem> 
static void BM_GenerateRandomSudoku(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        Sudoku sudoku;
        sudoku.generateRandomSudokuColored(2);
    }
}
BENCHMARK(BM_GenerateRandomSudoku);

BENCHMARK_MAIN();

