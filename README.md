Raylib Sorting Visualizer
===
A sorting algorithm visualizer created with [Raylib](https://github.com/raysan5/raylib) and C.

## Features

- `Array` struct pointer type for dealing with variable-length arrays (see [src/Array.c](src/Array.c) for documentation on how to use)
- `Algorithm` struct type for dealing with algorithms (contains a `bool (*)(Array)` function pointer and `const char *`)

## How to compile

I used Windows to create this but I'm sure it can be compiled on other platforms.

Before compiling, make sure you have a "bin" folder in the project directory.

For Windows, run this command:
```
gcc src\main.c -o bin\RaylibSortingVisualizer.exe -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -pthread -O2
```
To compile in debug mode, replace `-O2` with `-g`.

You can get GCC for Windows [here](https://www.mingw-w64.org/).

### Using my Makefile?

Please **read the warnings** at the top. Thank you!

## To-do

- [ ] Add indicator for where array accesses are, with different colors for reads, writes, and both
- [ ] Add sounds
- [ ] Add a HUD using a non-obtrusive color
- [ ] Add more algorithms

## What do I think of this?

I started this project in C, so I'm sticking with C, but man, do I wish I used [Rust](https://github.com/rust-lang/rust).