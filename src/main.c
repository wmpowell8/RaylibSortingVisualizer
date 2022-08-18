#include "rlonce.h"
#include <pthread.h>
#include "Array.c"
#include "algorithms/shuffle/StandardShuffle.c"
#include "algorithms/sort/BubbleSort.c"
#include "algorithms/sort/CocktailShakerSort.c"
#include "algorithms/sort/InsertionSort.c"
#include "algorithms/sort/SelectionSort.c"
#include "algorithms/sort/ICantBelieveItCanSort.c"
#include "algorithms/sort/QuickSort.c"
#include "algorithms/sort/MergeSort.c"
#include "algorithms/sort/BogoSort.c"

/**
 * @brief The delay to wait every time the sorting algorithm makes an array access
 */
float array_access_delay = 3.f;

/**
 * @brief The `Array` that the sorting algorithms act on
 */
Array sort_array;

void my_array_read_callback(Array array, size_t index)
{
    // TODO: make things work with external arrays

    if (array == sort_array)
    {
        WaitTime(array_access_delay);
    }
}

void my_array_write_callback(Array array, size_t index)
{
    // TODO: make things work with external arrays

    if (array == sort_array)
    {
        WaitTime(array_access_delay);
    }
}

/**
 * @brief Draws an `Array` onto the screen using Raylib
 */
void draw_array(Array array, int width, int height, int x, int y)
{
    // TODO: replace system for coloring in rectangles because the current system only uses the bottom 2 bits of every byte it allocates

    const Color RECTANGLE_COLORS[4] = {WHITE, BLUE, RED, MAGENTA};

    unsigned char *sort_array_modifications = NULL;
    if (array == sort_array)
    {
#ifdef SHOW_ACTIVITY
        sort_array_modifications = MemAlloc(array->len * sizeof(unsigned char));
        pthread_mutex_unlock(&sort_array_read_lock);
        for (size_t i = 0; i < sort_array_read_len; i++)
        {
            size_t index = sort_array_read_indices[i];
            if (index < array->len)
                sort_array_modifications[index] |= 1;
        }
        pthread_mutex_lock(&sort_array_read_lock);
        pthread_mutex_unlock(&sort_array_write_lock);
        for (size_t i = 0; i < sort_array_write_len; i++)
        {
            size_t index = sort_array_write_indices[i];
            if (index < array->len)
                sort_array_modifications[index] |= 2;
        }
        pthread_mutex_lock(&sort_array_write_lock);
#endif
    }

    for (size_t i = 0; i < array->len; i++)
    {
        int rect_height = (array->_arr[i] + 1) * height / array->len;
#ifdef SHOW_ACTIVITY
        Color rect_color = sort_array_modifications == NULL ? WHITE : RECTANGLE_COLORS[sort_array_modifications[i]];
#else
        const Color rect_color = WHITE;
#endif
        DrawRectangle(x + i * width / array->len, y + height - rect_height, width / array->len, rect_height, rect_color);
    }

    if (sort_array_modifications != NULL)
        MemFree(sort_array_modifications);
}

/**
 * @brief Demonstrates a sorting algorithm
 * 
 * @param sort The algorithm to demonstrate
 * @param array_size The size of the `Array` to demonstrate the algorithm on
 * @param shuffle The algorithm used to shuffle the `Array` before sorting
 * @return Whether the demonstration was successful
 */
bool show_sort(Algorithm sort, size_t array_size, Algorithm shuffle)
{
    WaitTime(750.f);
    SetWindowTitle("Sorting Visualizer - Initializing array");
    Array_free(sort_array);
    sort_array = Array_new_init(array_size);
    SetWindowTitle("Sorting Visualizer");

    WaitTime(750.f);
    SetRandomSeed(0);
    SetWindowTitle(TextFormat("Sorting Visualizer - %s", shuffle.name));
    if (!shuffle.fun(sort_array))
        return false;
    SetWindowTitle("Sorting Visualizer");

    WaitTime(750.f);
    SetRandomSeed(0);
    SetWindowTitle(TextFormat("Sorting Visualizer - %s", sort.name));
    if (!sort.fun(sort_array))
        return false;
    SetWindowTitle("Sorting Visualizer");

    return true;
}

/**
 * @brief The procedure used to demonstrate all the sorting algorithms
 * NOTE: The arguments and return value are not used; they are only there because this function is called in a new thread
 */
void *sort_proc(void *args)
{
    if (
        !show_sort(InsertionSort, 64, StandardShuffle) ||
        !show_sort(BubbleSort, 64, StandardShuffle) ||
        !show_sort(CocktailShakerSort, 64, StandardShuffle) ||
        !show_sort(SelectionSort, 64, StandardShuffle) ||
        !show_sort(ICantBelieveItCanSort, 32, StandardShuffle) ||
        !show_sort(MergeSort, 128, StandardShuffle) ||
        !show_sort(QuickSort, 128, StandardShuffle) ||
        !show_sort(BogoSort, 5, StandardShuffle))
    {
        TraceLog(LOG_ERROR, "Sorting Visualizer: algorithm returned false; stopped prematurely");
        return NULL;
    }
    return NULL;
}

int main()
{
    // SetTraceLogLevel(LOG_ALL);
    
    Array_set_at_callback(my_array_read_callback);
    Array_set_set_callback(my_array_write_callback);
    sort_array = Array_new_init(256);

    InitWindow(640, 480, "Sorting Visualizer");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(10, 10);

    pthread_t sort_thread;
    pthread_create(&sort_thread, NULL, sort_proc, NULL);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        draw_array(sort_array, GetScreenWidth() - 10, GetScreenHeight() - 10, 5, 5);

        EndDrawing();
    }

    CloseWindow();

    pthread_kill(sort_thread, SIGTERM);
    Array_free(sort_array);

    return 0;
}
