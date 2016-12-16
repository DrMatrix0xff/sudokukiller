from distutils.core import setup, Extension

module1 = Extension('sudoku',
        sources=['sudoku.c'],
        extra_compile_args = [
            '-I/usr/include/python2.7',
            '-I/usr/local/include/python2.7',
        ],
)

setup(
    name='SudokuSolver',
    version='1.0',
    description='A simple sudoku solver',
    ext_modules=[module1]
)
