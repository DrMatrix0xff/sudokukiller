program pfib;

var gr : longint;

function fib(n : integer) : longint;

var r : longint;
procedure FibIter(i : integer; a, b : longint; var r1 : longint);

begin
    if i = 0 then
        r1 := b
    else
        FibIter(i - 1, a + b, a, r1);
end;

begin
    FibIter(n, 1, 0, r);
    fib := r;
end;

BEGIN
    gr := fib(45);
    writeln('fib(45) = ', gr)
END.

