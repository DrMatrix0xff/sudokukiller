from distutils.core import setup, Extension

module1 = Extension('sudoku',
        sources=['sudoku.c']
        )

setup(
    name='SudokuSolver',
    version='1.0',
    description='A simple sudoku solver',
    ext_modules=[module1]
        )
