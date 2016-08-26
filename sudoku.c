#include <Python.h>
#include <assert.h>

typedef struct cell__ {
    int pos;
    int val;
} cell;

// error is set when procedure `solve` fails to find a solution.
static PyObject *Exc_Check, *Exc_Solve;

static int check(int puzzle[], int pos, int x)
{
    int line, col, box, topleft;
    int i, j;
    line = pos / 9;
    col = pos % 9;
    // calculate box number of which pos cell belongs to
    box = (line / 3) * 3 + col / 3;
    for (i = line * 9; i < (line+1) * 9; ++i)
        if (x == puzzle[i])
            return 0;
    for (i = col; i <= col + 72; i += 9)
        if (x == puzzle[i])
            return 0;
    topleft = (box / 3) * 27 + (box % 3) * 3;
    for (i = topleft; i <= topleft + 20; i += 9) {
        for (j = 0; j < 3; ++j)
            if (x == puzzle[i+j])
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

/* check if initial puzzle is valid */
static int check_initial_puzzle(int puzzle[], int n) {
    assert(n == 81);
    int i, j, k, topleft;
    int line, col, box;

    for (i = 0; i < n; i++) {
        if (puzzle[i] == 0)
            continue;
        line = i / 9;
        col = i % 9;
        box = (line / 3) * 3 + col / 3;
        for (j = 9*line; j < 9*line+9; j++)
            if ((j != i) && (puzzle[j] == puzzle[i]))
                return 0;
        for (j = col; j < 72+col; j += 9)
            if ((j != i) && (puzzle[j] == puzzle[i]))
                return 0;
        topleft = (box / 3) * 27 + (box % 3) * 3;
        for (j = topleft; j <= topleft+20; j += 9)
            for (k = 0; k < 3; k++)
                if (((j+k) != i) && (puzzle[i] == puzzle[j+k]))
                    return 0;
    }
    fprintf(stderr, "initial check done\n");
    return 1;
}

// return 0 when solved successfully, otherwise non-zero
static int solve(int puzzle[])
{
    int i;
    int v;
    cell c;
    cell stack[81] = {(cell){.pos=0, .val=0}};
    int spi = 0;
    for ( ; ; ) {
        for (i = 0; i <= 80; ++i)
            if (puzzle[i] == 0)
                break;
        if (i >= 81) {
            return 0;
        }
        v = make_guess(puzzle, i, 0);
        if (v <= 9) {
            puzzle[i] = v;
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
#ifndef NDEBUG
                fprintf(stderr, "bad input sudoku!\n");
#endif
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
    fprintf(stderr, "ref count of args: %d\n", (int)args->ob_refcnt);
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
    fprintf(stderr, "ref count of o: %d\n", (int)o->ob_refcnt);
    if ((vec = (int *)calloc(81, sizeof(int))) == NULL) {
        fprintf(stderr, "memory alloc failed\n");
        return ob;
    }
    for (i = 0; i < 81; ++i) {
        ob = PyList_GetItem(o, i);
        if ((n = PyInt_AS_LONG(ob)) == -1) {
            fprintf(stderr, "parse list item failed\n");
            goto failure;
        }
        vec[i] = (int)n;
    }
    if (! check_initial_puzzle(vec, 81)) {
        fprintf(stderr, "ref count of Exc_Check: %d\n", (int)Exc_Check->ob_refcnt);
        PyErr_SetString(Exc_Check, "bad initial input puzzle");
        goto failure;
    }
    if (solve(vec) == 0) {
        ob = PyList_New(len);
        if (! ob) {
            fprintf(stderr, "build PyList failed\n");
            goto failure;
        }
        for (i = 0; i < 81; ++i) {
            // o = Py_BuildValue("i", vec[i]);
            if (PyList_SetItem(ob, (Py_ssize_t)i, Py_BuildValue("i", vec[i])) < 0) {
                fprintf(stderr, "build PyInt value failed\n");
                goto failure;
            }
        }
        free(vec);
        fprintf(stderr, "ref count of ob: %d\n", (int)ob->ob_refcnt);
        return ob;
    } else {
        // solve(vec) == 1
        PyErr_SetString(Exc_Solve, "It seems no proper solution");
    }

failure:
    if (PyErr_Occurred()) {
        fprintf(stderr, "PyErr Occurs\n");
        // PyErr_Clear();
    }
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
    PyObject *m;
    m = Py_InitModule("sudoku", SudokuMethods);
    if (m == NULL)
        return;
    Exc_Check = PyErr_NewException("sudoku.CheckException", NULL, NULL);
    Exc_Solve = PyErr_NewException("sudoku.SolveException", NULL, NULL);
    Py_INCREF(Exc_Check);
    Py_INCREF(Exc_Solve);
    PyModule_AddObject(m, "CheckException", Exc_Check);
    PyModule_AddObject(m, "SolveException", Exc_Solve);
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
