
# Assumptions

- Using a modern C compiler, e.g. gcc, capable of function inlining.
- cmake

# Compiling

```
$ cmake .
$ make
```

# Running

```
$ ./bin/maze file1 file2 ... fileN
```

# Demo

```
$ ./bin/maze inputs/maze1
inputs/maze1:
> preprocessing
> allocating
> loading 9 x 14 maze
> solving
 -2/b -2/b -2/b -2/b -2/b -2/b 11/* -2/b -2/b -2/b -2/b -2/b -2/b -2/b
 -2/b -2/b 10/*  9/*  8/*  9/* 10/* 11/* 12/* 13/* 14/* -2/b -2/b -2/b
 -2/b -2/b 11/* -2/b  7/* -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b
 -2/b -2/b 12/* -2/b  6/*  5/*  4/*  3/*  2/*  1/*  0/* -2/b -2/b -2/b
 -2/b -2/b 13/* -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b
 -2/b -2/b 14/* 15/* 16/* 17/* 18/* -2/b 24/* 25/* 26/* -2/b -2/b -2/b
 -2/b -2/b -2/b -2/b -2/b -2/b 19/* -2/b 23/* -2/b 27/* -2/b -2/b -2/b
 -2/b -2/b -1/? -1/? -2/b 21/* 20/* 21/* 22/* -2/b 28/* -2/b -2/b -2/b
 -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b -2/b
```
