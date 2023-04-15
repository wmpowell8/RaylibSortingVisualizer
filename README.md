# Raylib Sorting Visualizer

A sorting algorithm visualizer created with [Raylib](https://github.com/raysan5/raylib) and C.

## Features

- `Array` struct pointer type for dealing with variable-length arrays (see [src/Array.c](src/Array.c) for documentation on how to use)
- `Algorithm` struct type for dealing with algorithms (contains a `bool (*)(Array)` function pointer and `const char *`)
- [Procedurally-generated audio](src/procedural_audio.c) for clean sounds

## How to compile

I used Windows to create this but I'm sure it can be compiled on other platforms.

For Windows, run this command:

```cmd
gcc src\main.c -o RaylibSortingVisualizer.exe -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -pthread -O2
```

To compile in debug mode, replace `-O2` with `-g`.

You can get GCC for Windows [here](https://www.mingw-w64.org/).

### Using my Makefile?

Please **read the warnings** at the top. Thank you!

## ⚠️ Warning 🚧

Raylib Sorting Visualizer's [procedurally-generated audio](src/procedural_audio.c) (introduced in [commit a65fb36](https://github.com/wmpowell8/RaylibSortingVisualizer/commit/a65fb3606934e5fe04aa2691753df83079aefd3f)), when initializing, can sometimes cause the Windows Audio service to malfunction, preventing any audio from playing until it is restarted.

To fix this issue, you can restart the Windows Audio service by doing the following:

1. Simultaneously press the Windows key and R and type in "services.msc".
2. Ensure "Services (Local)" is selected on the left and scroll down until you see "Windows Audio". Select it.
3. Click on the "Restart Service" button at the top. It's the one with a rectangle and a triangle.

## To-do

- [X] Add indicator for where array accesses are, with different colors for reads, writes, and both ([commit bc540cc](https://github.com/wmpowell8/RaylibSortingVisualizer/commit/bc540cc1ceae5de769dd40eda9566bbea58ca124))
  - [X] Make the indicator more visible ([commit f44feb7](https://github.com/wmpowell8/RaylibSortingVisualizer/commit/f44feb734cb0bacd6af8d58c7e7b7ead949563d5))
- [X] Add sounds ([commit ad13cd8](https://github.com/wmpowell8/RaylibSortingVisualizer/commit/ad13cd830af638f578367f0773e4142bc5ea66d0))
- [X] Add a HUD using a non-obtrusive color ([commit ce77fdc](https://github.com/wmpowell8/RaylibSortingVisualizer/commit/ce77fdc2f51498ac3e74572db742909b3b439866))
- [ ] Add more algorithms

### Contributing new algorithms

To contribute new algorithms, first read the documentation comments in [src/Array.c](src/Array.c) to get an idea of how this visualizer implements algorithms and dynamically-sized arrays. Then go into the [src/algorithms/sort](src/algorithms/sort) folder and create a new file. In this file you will define a new `Algorithm` which contains the data for the sorting algorithm you're implementing. Finally, go into [src/main.c](src/main.c), include the file you just created, and add a new call to `show_sort` under the long if-statement in the `sort_proc` function. For information on which arguments you should put into the `show_sort` call to make your new algorithm show properly, consult the documentation comments on `show_sort`.

## What do I think of this?

I started this project in C, so I'm sticking with C, but man, do I wish I used [Rust](https://github.com/rust-lang/rust).
