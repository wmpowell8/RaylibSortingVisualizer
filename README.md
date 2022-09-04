Raylib Sorting Visualizer
===
A sorting algorithm visualizer created with [Raylib](https://github.com/raysan5/raylib) and C.

## Features

- `Array` struct pointer type for dealing with variable-length arrays (see [src/Array.c](src/Array.c) for documentation on how to use)
- `Algorithm` struct type for dealing with algorithms (contains a `bool (*)(Array)` function pointer and `const char *`)

## How to compile

I used Windows to create this but I'm sure it can be compiled on other platforms.

For Windows, run this command:
```
gcc src\main.c -o RaylibSortingVisualizer.exe -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -pthread -O2
```
To compile in debug mode, replace `-O2` with `-g`.

You can get GCC for Windows [here](https://www.mingw-w64.org/).

### Using my Makefile?

Please **read the warnings** at the top. Thank you!

## To-do

- [X] Add indicator for where array accesses are, with different colors for reads, writes, and both ([commit bc540cc](https://github.com/wmpowell8/RaylibSortingVisualizer/commit/bc540cc1ceae5de769dd40eda9566bbea58ca124))
  - [ ] Make the indicator more visible
- [X] Add sounds ([commit ad13cd8](https://github.com/wmpowell8/RaylibSortingVisualizer/commit/ad13cd830af638f578367f0773e4142bc5ea66d0))
- [ ] Add a HUD using a non-obtrusive color
- [ ] Add more algorithms

## What do I think of this?

I started this project in C, so I'm sticking with C, but man, do I wish I used [Rust](https://github.com/rust-lang/rust).
