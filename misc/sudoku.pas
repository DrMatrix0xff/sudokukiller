program sudokuSolution;

type cellEntry = record
    position : integer;
    value : integer;
end;

var puzzle : array [1..81] of integer =
(
(*
0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, 0,0,0
*)
(*
0,0,3, 0,0,0, 0,1,0,
0,1,0, 0,3,6, 7,0,0,
0,2,8, 0,0,5, 0,0,0,
0,0,0, 8,0,0, 5,3,0,
0,0,1, 6,0,0, 0,0,0,
0,0,0, 5,0,0, 6,7,0,
0,4,6, 0,0,7, 0,0,0,
0,3,0, 0,4,8, 9,0,0,
0,0,2, 0,0,0, 0,4,0
*)
0,0,0, 0,0,0, 0,0,0,
9,1,0, 7,0,0, 0,0,0,
7,0,0, 0,0,8, 0,0,0,
0,0,0, 0,0,5, 0,0,0,
0,0,4, 0,8,2, 0,0,0,
6,0,0, 0,0,0, 9,1,0,
0,0,5, 0,0,0, 0,0,8,
0,0,0, 0,0,0, 0,0,2,
0,0,0, 1,0,0, 0,7,0
);

    isDone : boolean;
    idx : integer;

procedure checkValid(posi : integer; valu : integer; var isValid : boolean);

var
    lno, col, box : integer;
    boxcol : integer;
    k : integer;
    i : integer;
begin
    lno := (posi + 8) div 9;
    col := posi mod 9;
    if col = 0 then
        col := 9;
    box := ((lno - 1) div 3) * 3 + (col + 2) div 3;
    i := 9 * lno - 8;
    while i <= 9 * lno do begin
        if valu = puzzle[i] then begin
            isValid := false;
            exit
        end;
        inc(i)
    end;
    i := col;
    while i <= col + 72 do begin
        if valu = puzzle[i] then begin
            isValid := false;
            exit
        end;
        i := i + 9;
    end;
    boxcol := box mod 3;
    if boxcol = 0 then
        boxcol := 3;
    i := 27 * ((box - 1) div 3) + 3 * boxcol - 2;
    while i <= 27 * ((box - 1) div 3) + 3 * boxcol + 18 do begin
        for k := 0 to 2 do
            if valu = puzzle[i+k] then begin
                isValid := false;
                exit
            end;
        i := i + 9
    end;
    isValid := true

end;

procedure makeGuess(begv : integer; posi : integer; var resv : integer);

var r : integer;
var b : boolean;

begin
    r := begv + 1;
    while r <= 9 do begin
        checkValid(posi, r, b);
        if b then begin
            resv := r;
            exit
        end;
        inc(r);
    end;
    resv := r
end;

procedure findSolution(var resb : boolean);

var c : cellEntry;
var stk : array [1..81] of cellEntry;
var sp : integer;
var p: integer;
var v : integer;

begin
    sp := 1;
    while true do begin
        p := 1;
        while p <= 81 do begin
            if puzzle[p] = 0 then
                break;
            inc(p)
        end;
        if p >= 82 then begin
            resb := true;
            exit
        end;
        makeGuess(0, p, v);
        (* writeln('v: ', v, ' p: ', p); *)
        if v <= 9 then begin
            puzzle[p] := v;
            c.position := p;
            c.value := v;
            stk[sp] := c;
            inc(sp)
        end
        else begin
            (* writeln('stack depth: ', sp); *)
            while sp > 1 do begin
                dec(sp);
                c := stk[sp];
                makeGuess(c.value, c.position, v);
                if v <= 9 then begin
                    puzzle[c.position] := v;
                    c.value := v;
                    stk[sp] := c;
                    inc(sp);
                    break;
                end
                else
                    puzzle[c.position] := 0;
            end;
            if sp <= 1 then begin
                writeln('no solution could be found');
                resb := false;
                exit
            end;
        end;
    end;
end;

begin
    findSolution(isDone);
    if isDone then begin
        for idx := 1 to 81 do begin
            if idx mod 9 = 0 then
                writeln(puzzle[idx])
            else
                write(puzzle[idx], ' ')
        end;
    end;
end.

