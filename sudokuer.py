#!/usr/bin/env python
# coding: utf-8

from collections import namedtuple

Entry = namedtuple('Entry', 'pos val')

def make_sudoku_solver(puzzle):
    solver = SudokuSolver()
    solver.set_sudoku(puzzle)
    if solver.check_sudoku():
        return solver
    raise Exception("invalid input puzzle")

class SudokuSolver(object):
    def __init__(self):
        self.puzzle = []
        self.stack = []
        #self.counter = 0

    def set_sudoku(self, puzzle):
        self.puzzle = puzzle

    def check_sudoku(self):
        # TODO
        return True

    def __check_value(self, pos, x):
        line = pos / 9
        col = pos % 9
        box = (line / 3) * 3 + col / 3
        i = line * 9
        while i < (line+1) * 9:
            if self.puzzle[i] != 0 and self.puzzle[i] == x:
                return False
            i += 1
        i = col
        while i <= (col + 72):
            if self.puzzle[i] != 0 and self.puzzle[i] == x:
                return False
            i += 9
        topleft = (box / 3) * 27 + (box % 3) * 3
        i = topleft
        while i <= (topleft+20):
            for j in range(3):
                if self.puzzle[i+j] != 0 and self.puzzle[i+j] == x:
                    return False
            i += 9
        return True

    def make_guess(self, pos, b):
        guess = b+1
        while guess <= 9:
            if self.__check_value(pos, guess):
                return guess
            guess += 1
        return guess

    def solve(self):
        while 1:
            i = 0
            while i < 81:
                if self.puzzle[i] == 0:
                    print '.',
                    break
                i += 1
            if i >= 81:
                print ("done!")
                break
            v = self.make_guess(i, 0)
            #print ';; v', v
            if v <= 9:
                self.puzzle[i] = v
                self.stack.append(Entry(i, v))
            else:
                while len(self.stack) > 0:
                    #print 'stack depth:', len(self.stack)
                    e = self.stack.pop()
                    v = self.make_guess(e.pos, e.val)
                    #print ';; adjust v', v
                    if v <= 9:
                        self.puzzle[e.pos] = v
                        self.stack.append(Entry(e.pos, v))
                        break
                    else:
                        # reset the cell referred by top of the stack
                        self.puzzle[e.pos] = 0
                if len(self.stack) == 0:
                    raise Exception("bad sudoku, no solution")

    def show_solution(self):
        lines = []
        for i in range(81):
            if i % 9 == 0:
                l = []
                l.append(str(self.puzzle[i]))
            elif (i+1) % 9 == 0:
                l.append(str(self.puzzle[i]))
                lines.append(' '.join(l))
            else:
                l.append(str(self.puzzle[i]))
        print '\n'.join(lines)

if __name__ == '__main__':
    sdksolver = make_sudoku_solver([
        #0,3,0, 0,0,4, 5,0,0,
        #0,0,7, 6,0,0, 0,0,2,
        #0,0,1, 0,0,9, 0,0,0,
        #0,1,0, 0,0,3, 8,0,7,
        #0,8,6, 0,9,0, 0,0,4,
        #0,0,0, 0,0,0, 0,0,0,
        #0,0,2, 0,8,0, 0,0,5,
        #0,0,0, 0,2,0, 3,0,0,
        #0,5,0, 4,0,0, 7,0,0,
        #===================
        9,3,0, 0,0,0, 4,8,0,
        0,0,4, 0,0,0, 0,0,3,
        6,0,1, 0,3,0, 0,9,5,
        1,0,0, 3,0,8, 0,5,0,
        0,0,0, 1,9,6, 0,0,0,
        0,0,0, 7,2,5, 0,0,0,
        0,0,2, 0,0,0, 0,0,9,
        8,4,0, 0,0,0, 5,1,0,
        5,0,9, 0,8,0, 0,4,7,
    ])
    sdksolver.solve()
    sdksolver.show_solution()
