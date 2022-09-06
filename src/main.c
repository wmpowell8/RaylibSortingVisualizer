#include "rlonce.h"
#include <pthread.h>
#include "Array.c"
#include "algorithms/shuffle/StandardShuffle.c"
#include "algorithms/sort/BubbleSort.c"
#include "algorithms/sort/CocktailShakerSort.c"
#include "algorithms/sort/InsertionSort.c"
#include "algorithms/sort/SelectionSort.c"
#include "algorithms/sort/OddEvenSort.c"
#include "algorithms/sort/ICantBelieveItCanSort.c"
#include "algorithms/sort/QuickSort.c"
#include "algorithms/sort/MergeSort.c"
#include "algorithms/sort/RadixSortLSD.c"
#include "algorithms/sort/HeapSort.c"
#include "algorithms/sort/SlowSort.c"
#include "algorithms/sort/BogoSort.c"

/**
 * @brief The delay to wait every time the sorting algorithm makes an array access
 */
float array_access_delay = 1.5f;

/**
 * @brief The `Array` that the sorting algorithms act on
 */
Array sort_array;

pthread_t sort_array_read_lock = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
size_t sort_array_read_len = 0;
size_t *sort_array_read_indices;

pthread_t sort_array_write_lock = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
size_t sort_array_write_len = 0;
size_t *sort_array_write_indices;

Sound array_read_sound;
Sound array_write_sound;

char status_text[256] = "";

size_t array_read_count = 0;
size_t array_write_count = 0;

#define push_array_access(mutex, index_stack, index_len)               \
    pthread_mutex_lock(mutex);                                         \
    size_t old_len = index_len;                                        \
    index_len++;                                                       \
    index_stack = MemRealloc(index_stack, index_len * sizeof(size_t)); \
    index_stack[old_len] = index;                                      \
    pthread_mutex_unlock(mutex);

void my_array_read_callback(Array array, size_t index)
{
    // TODO: make things work with external arrays

    if (array == sort_array)
    {
        push_array_access(&sort_array_read_lock, sort_array_read_indices, sort_array_read_len);
        array_read_count++;
        WaitTime(array_access_delay);
    }
}

void my_array_write_callback(Array array, size_t index)
{
    // TODO: make things work with external arrays

    if (array == sort_array)
    {
        push_array_access(&sort_array_write_lock, sort_array_write_indices, sort_array_write_len);
        array_write_count++;
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

#define compile_array_accesses(mutex, index_stack, index_len, bit, sound)      \
    pthread_mutex_lock(mutex);                                                 \
    for (size_t i = 0; i < index_len; i++)                                     \
    {                                                                          \
        size_t index = index_stack[i];                                         \
        if (index < array->len)                                                \
        {                                                                      \
            sort_array_modifications[index >> 2] |= bit << ((index & 3) << 1); \
            SetSoundPitch(sound, (float)array->_arr[index] / array->len);      \
            PlaySound(sound);                                                  \
        }                                                                      \
    }                                                                          \
    index_len = 0;                                                             \
    index_stack = MemRealloc(index_stack, 0);                                  \
    pthread_mutex_unlock(mutex);

    unsigned char *sort_array_modifications = NULL;
    if (array == sort_array)
    {
        sort_array_modifications = MemAlloc((array->len + 3 >> 2) * sizeof(unsigned char));

        compile_array_accesses(&sort_array_read_lock, sort_array_read_indices, sort_array_read_len, 1, array_read_sound);
        compile_array_accesses(&sort_array_write_lock, sort_array_write_indices, sort_array_write_len, 2, array_write_sound);
    }

    for (size_t i = 0; i < array->len; i++)
    {
        int rect_height = (array->_arr[i] + 1) * height / array->len;
        Color rect_color = sort_array_modifications == NULL ? WHITE : RECTANGLE_COLORS[sort_array_modifications[i >> 2] >> ((i & 3) << 1) & 3];
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
    status_text[255] = '\0';

    WaitTime(750.f);
    array_read_count = 0;
    array_write_count = 0;
    strcpy_s(status_text, 255, "Initializing array");
    Array_free(sort_array);
    sort_array = Array_new_init(array_size);
    strcpy_s(status_text, 255, "");

    WaitTime(750.f);
    array_read_count = 0;
    array_write_count = 0;
    SetRandomSeed(0);
    strcpy_s(status_text, 255, TextFormat("Shuffling: %s", shuffle.name));
    if (!shuffle.fun(sort_array))
        return false;
    strcpy_s(status_text, 255, "");

    WaitTime(750.f);
    array_read_count = 0;
    array_write_count = 0;
    SetRandomSeed(0);
    strcpy_s(status_text, 255, TextFormat("Sorting: %s", sort.name));
    if (!sort.fun(sort_array))
        return false;
    strcpy_s(status_text, 255, "");

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
        !show_sort(SelectionSort, 96, StandardShuffle) ||
        !show_sort(ICantBelieveItCanSort, 48, StandardShuffle) ||
        !show_sort(OddEvenSort, 64, StandardShuffle) ||
        !show_sort(MergeSort, 256, StandardShuffle) ||
        !show_sort(QuickSort, 128, StandardShuffle) ||
        !show_sort(RadixSortLSD, 512, StandardShuffle) ||
        !show_sort(HeapSort, 128, StandardShuffle) ||
        !show_sort(SlowSort, 24, StandardShuffle) ||
        !show_sort(BogoSort, 5, StandardShuffle))
    {
        TraceLog(LOG_ERROR, "Sorting Visualizer: algorithm returned false; stopped prematurely");
        return NULL;
    }
    return NULL;
}

int main()
{
    SetTraceLogLevel(LOG_ALL);

    sort_array_read_indices = MemAlloc(0);
    sort_array_write_indices = MemAlloc(0);

    Array_set_at_callback(my_array_read_callback);
    Array_set_set_callback(my_array_write_callback);
    sort_array = Array_new_init(256);

    InitAudioDevice();
    array_read_sound = LoadSound("assets/array_read.wav");
    array_write_sound = LoadSound("assets/array_write.wav");

    InitWindow(640, 480, "Sorting Visualizer");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(10, 10);
    SetTargetFPS(60);

    pthread_t sort_thread;
    pthread_create(&sort_thread, NULL, sort_proc, NULL);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        draw_array(sort_array, GetScreenWidth() - 10, GetScreenHeight() - 10, 5, 5);
        DrawText(TextFormat("%s\nArray Accesses: %llu\n\t(%llu reads, %llu writes)", status_text, array_read_count + array_write_count, array_read_count, array_write_count), 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();

    pthread_kill(sort_thread, SIGTERM);
    Array_free(sort_array);

    MemFree(sort_array_read_indices);
    MemFree(sort_array_write_indices);

    return 0;
}
