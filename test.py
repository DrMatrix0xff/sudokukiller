#!/usr/bin/env python
# coding: utf-8

import sys
import sudoku

problem = [
    #5,0,0, 0,0,6, 0,0,0,
    #0,0,9, 0,0,0, 0,0,8,
    #2,0,0, 0,0,8, 0,0,0,
    #0,4,5, 0,0,0, 0,0,0,
    #0,0,3, 0,0,0, 0,0,0,
    #0,0,6, 0,0,3, 0,5,4,
    #0,0,0, 3,2,5, 0,0,6,
    #0,0,0, 0,0,0, 0,0,0,
    #0,0,0, 0,0,0, 0,0,0,
#=======================
    #0,8,3, 0,2,6, 0,0,0,
    #0,0,0, 8,0,0, 1,0,0,
    #0,0,9, 0,0,0, 0,2,0,
    #0,0,0, 0,6,0, 0,5,9,
    #0,0,1, 7,0,2, 6,0,0,
    #6,2,0, 0,9,0, 0,0,0,
    #0,7,0, 0,0,0, 9,0,0,
    #0,0,2, 0,0,3, 0,0,0,
    #0,0,0, 9,1,0, 5,7,0,
#=======================
    #4,0,0, 0,0,3, 7,0,0,
    #3,0,6, 0,0,0, 0,0,0,
    #0,9,0, 0,8,7, 0,0,0,
    #0,0,9, 0,2,0, 0,0,3,
    #0,0,1, 0,0,0, 8,0,0,
    #8,0,0, 0,7,0, 6,0,0,
    #0,0,0, 4,9,0, 0,8,0,
    #0,0,0, 0,0,0, 1,0,9,
    #0,0,2, 6,0,0, 0,0,4,
#=======================
    0,0,8, 0,0,7, 1,0,0,
    0,0,0, 8,0,0, 0,4,0,
    5,0,6, 1,0,0, 3,0,0,
    0,0,3, 0,0,4, 0,0,9,
    7,0,0, 0,0,0, 0,0,3,
    9,0,0, 7,0,0, 4,0,0,
    0,0,1, 0,0,2, 6,0,5,
    0,6,0, 0,0,8, 0,0,0,
    0,0,2, 3,0,0, 8,0,0,
]

print 'ref count of var problem:', sys.getrefcount(problem)
non_zero_indice = []

for i, v in enumerate(problem, 1):
    if v != 0:
        non_zero_indice.append(i)

redchar = '\x1b[31m'
norchar = '\x1b[0m'

a = sudoku.solve(problem)
print 'ref count of var problem:', sys.getrefcount(problem)

#print problem
lsep = ' | '.join(['_' * 5] * 3) + '\n'
ssep = ' | '.join([' ' * 5] * 3) + '\n'
print ''
for i, x in enumerate(a, 1):
    if i % 9 == 0:
        if i in non_zero_indice:
            c = redchar + str(x) + norchar
        else:
            c = str(x)
        sys.stdout.write(c + '\n')
    elif i % 3 == 0:
        if i in non_zero_indice:
            c = redchar + str(x) + norchar
        else:
            c = str(x)
        sys.stdout.write(c + ' | ')
    else:
        if i in non_zero_indice:
            c = redchar + str(x) + norchar
        else:
            c = str(x)
        sys.stdout.write(c + ' ')
    if i % 27 == 0 and i < 81:
        sys.stdout.write(lsep+ssep)
print ''
