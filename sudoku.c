#include <python2.7/Python.h>
#include <stdio.h>

typedef struct cell_ {
    int pos;
    int val;
} cell;

static cell stack[81];
static int spi;

static int check(int puzzle[], int pos, int x)
{
    int line, col, box, topleft;
    int i, j;
    line = pos / 9;
    col = pos % 9;
    // calculate box number of which pos cell belongs to
    box = (line / 3) * 3 + col / 3;
    for (i = line * 9; i < (line+1) * 9; ++i)
        if (x == puzzle[i] && puzzle[i] != 0)
            return 0;
    for (i = col; i <= col + 72; i += 9)
        if (x == puzzle[i] && puzzle[i] != 0)
            return 0;
    topleft = (box / 3) * 27 + (box % 3) * 3;
    for (i = topleft; i <= topleft + 20; i += 9) {
        for (j = 0; j < 3; ++j)
            if (x == puzzle[i+j] && puzzle[i+j] != 0)
                return 0;
    }
    return 1;
}

static int make_guess(int puzzle[], int pos, int b)
{
    int i;
    for (i = b + 1; i <= 9; ++i)
        if(check(puzzle, pos, i) != 0)
            return i;
    return i;
}

// return 0 when solved successfully, otherwise non-zero
static int solve(int puzzle[])
{
    int i;
    int v;
    cell c;
    for ( ; ; ) {
        for (i = 0; i <= 80; ++i)
            if (puzzle[i] == 0)
                break;
        if (i >= 81)
            return 0;
        v = make_guess(puzzle, i, 0);
        if (v <= 9) {
            puzzle[i] = v;
            // how to cons a cell in C
            stack[spi++] = (cell){.pos=i, .val=v};
        } else {
            while (spi > 0) {
                c = stack[--spi];
                v = make_guess(puzzle, c.pos, c.val);
                if (v <= 9) {
                    puzzle[c.pos] = v;
                    stack[spi++] = (cell){.pos=c.pos, .val=v};
                    break;
                } else
                    // reset c.pos to 0;
                    puzzle[c.pos] = 0;
            }
            if (spi <= 0) {
                fprintf(stderr, "bad input sudoku!\n");
                return 1;
            }
        }

    }
    // make compiler happy
    return 1;
}


static PyObject *sudoku_solve(PyObject *self, PyObject *args)
{
    int i, *vec;
    long n;
    Py_ssize_t len;
    PyObject *ob = NULL, *o;
    if (!PyArg_ParseTuple(args, "O", &o)) {
        fprintf(stderr, "parse args failed\n");
        return ob;
    }
    if (!PyList_Check(o)) {
        fprintf(stderr, "check list failed\n");
        return ob;
    }
    if ((len = PyList_Size(o)) != (Py_ssize_t)81) {
        fprintf(stderr, "check list size failed\n");
        return ob;
    }
    if ((vec = (int *)calloc(81, sizeof(int))) == NULL)
        return ob;
    for (i = 0; i < 81; ++i) {
        ob = PyList_GetItem(o, i);
        if ((n = PyInt_AS_LONG(ob)) == -1) {
            fprintf(stderr, "parse list item failed\n");
            goto failed;
        }
        vec[i] = (int)n;
    }
    if (solve(vec) == 0) {
        ob = PyList_New(len);
        for (i = 0; i < 81; ++i) {
            o = Py_BuildValue("i", vec[i]);
            if (PyList_SetItem(ob, (Py_ssize_t)i, o) < 0)
                goto failed;
        }
        return ob;
    }

failed:
    free(vec);
    return NULL;
}

static PyMethodDef SudokuMethods[] = {
    {"solve", sudoku_solve, METH_VARARGS, "solve a sudoku puzzle"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initsudoku(void)
{
    (void) Py_InitModule("sudoku", SudokuMethods);
}

// int main(int argc, char *argv[])
// {
//     int i;
//     int problem[81] = {
//         0,0,0, 0,0,6, 0,0,0,
//         0,5,9, 0,0,0, 0,0,8,
//         2,0,0, 0,0,8, 0,0,0,
//         0,4,5, 0,0,0, 0,0,0,
//         0,0,3, 0,0,0, 0,0,0,
//         0,0,6, 0,0,3, 0,5,4,
//         0,0,0, 3,2,5, 0,0,6,
//         0,0,0, 0,0,0, 0,0,0,
//         0,0,0, 0,0,0, 0,0,0
//     };
//     if (solve(problem) == 0) {
//         for (i = 0; i < 81; ++i) {
//             printf("%d", problem[i]);
//             if ((i % 9) == 8) {
//                 putchar('\n');
//             } else {
//                 putchar(' ');
//             }
//         }
//     }
//     return 0;
// }
