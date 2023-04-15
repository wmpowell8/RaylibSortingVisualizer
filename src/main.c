#include "rlonce.h"
#include <pthread.h>
#include "Array.c"
#include "procedural_audio.c"
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
 * @brief The delay to wait every time the sorting algorithm makes an array access (in milliseconds)
 */
float array_access_delay = 2.f;

#define SOUND_SUSTAIN 0.05f

/**
 * @brief Used in the pause_for macro, which waits until clock() exceeds this value
 */
float pause;
/**
 * @brief Intended to be used in a single thread and no other. Waits until `ms` milliseconds since the last pause_for call.
 */
#define pause_for(ms)                    \
    pause += ms * CLOCKS_PER_SEC / 1000; \
    while (clock() < pause)              \
        sched_yield();

/**
 * @brief The `Array` that the sorting algorithms act on
 */
Array sort_array;

pthread_mutex_t sort_array_read_lock = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
size_t sort_array_read_len = 0;
size_t *sort_array_read_indices;

pthread_mutex_t sort_array_write_lock = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
size_t sort_array_write_len = 0;
size_t *sort_array_write_indices;

char status_text[256] = "";

size_t array_read_count = 0;
size_t array_write_count = 0;

#define push_array_access(mutex, index_stack, index_len, waveform)     \
    pthread_mutex_lock(mutex);                                         \
    size_t old_len = index_len;                                        \
    index_len++;                                                       \
    index_stack = MemRealloc(index_stack, index_len * sizeof(size_t)); \
    index_stack[old_len] = index;                                      \
    pthread_mutex_unlock(mutex);                                       \
    push_sound(waveform, array_access_delay / 500 / SOUND_SUSTAIN, (float)array->_arr[index] / array->len, SOUND_SUSTAIN);

void my_array_read_callback(Array array, size_t index)
{
    // TODO: make things work with external arrays

    if (array == sort_array)
    {
        push_array_access(&sort_array_read_lock, sort_array_read_indices, sort_array_read_len, sine_wave);
        array_read_count++;
        pause_for(array_access_delay);
    }
}

void my_array_write_callback(Array array, size_t index)
{
    // TODO: make things work with external arrays

    if (array == sort_array)
    {
        push_array_access(&sort_array_write_lock, sort_array_write_indices, sort_array_write_len, sawtooth_wave);
        array_write_count++;
        pause_for(array_access_delay);
    }
}

/**
 * @brief Draws an `Array` onto the screen using Raylib
 */
void draw_array(Array array, int width, int height, int x, int y)
{
    const Color RECTANGLE_COLORS[4] = {WHITE, BLUE, RED, MAGENTA};

#define compile_array_accesses(mutex, index_stack, index_len, bit)         \
    pthread_mutex_lock(mutex);                                             \
    for (size_t i = 0; i < index_len; i++)                                 \
    {                                                                      \
        size_t index = index_stack[i];                                     \
        if (index >= array->len)                                           \
            continue;                                                      \
        sort_array_modifications[index >> 2] |= bit << ((index & 3) << 1); \
    }                                                                      \
    index_len = 0;                                                         \
    index_stack = MemRealloc(index_stack, 0);                              \
    pthread_mutex_unlock(mutex);

    unsigned char *sort_array_modifications = NULL;
    if (array == sort_array)
    {
        sort_array_modifications = MemAlloc((array->len) * sizeof(unsigned char));
        compile_array_accesses(&sort_array_read_lock, sort_array_read_indices, sort_array_read_len, 1);
        compile_array_accesses(&sort_array_write_lock, sort_array_write_indices, sort_array_write_len, 2);
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
 * @param delay The delay between array accesses
 * @param shuffle The algorithm used to shuffle the `Array` before sorting
 * @return Whether the demonstration was successful
 */
bool show_sort(Algorithm sort, size_t array_size, float delay, Algorithm shuffle)
{
    status_text[255] = '\0';

    pause_for(750.f);
    array_read_count = 0;
    array_write_count = 0;
    strcpy_s(status_text, 255, TextFormat("Initializing %llu-element array", array_size));
    float old_d = array_access_delay;
    array_access_delay = 0.f; // for instant array initialization
    Array_free(sort_array);
    sort_array = Array_new_init(array_size);
    array_access_delay = old_d;
    strcpy_s(status_text, 255, "");

    pause_for(750.f);
    array_read_count = 0;
    array_write_count = 0;
    SetRandomSeed(0);
    strcpy_s(status_text, 255, TextFormat("Shuffling: %s (%llu elements)", shuffle.name, array_size));
    old_d = array_access_delay;
    array_access_delay = 500.f / 4 / array_size; // 4 array accesses required per element when shuffling
    if (!shuffle.fun(sort_array))
        return false;
    array_access_delay = old_d;
    strcpy_s(status_text, 255, "");

    pause_for(750.f);
    array_read_count = 0;
    array_write_count = 0;
    SetRandomSeed(0);
    strcpy_s(status_text, 255, TextFormat("Sorting: %s (%llu elements)", sort.name, array_size));
    old_d = array_access_delay;
    array_access_delay = delay;
    if (!sort.fun(sort_array))
        return false;
    array_access_delay = old_d;
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
        !show_sort(InsertionSort, 96, 1.447f, StandardShuffle) ||
        !show_sort(BubbleSort, 64, 1.628f, StandardShuffle) ||
        !show_sort(CocktailShakerSort, 64, 1.628f, StandardShuffle) ||
        !show_sort(SelectionSort, 96, 2.003f, StandardShuffle) ||
        !show_sort(ICantBelieveItCanSort, 48, 1.447f, StandardShuffle) ||
        !show_sort(OddEvenSort, 48, 1.447f, StandardShuffle) ||
        !show_sort(MergeSort, 256, 2.441f, StandardShuffle) ||
        !show_sort(QuickSort, 192, 2.193f, StandardShuffle) ||
        !show_sort(RadixSortLSD, 512, 1.953f, StandardShuffle) ||
        !show_sort(HeapSort, 128, 1.6f, StandardShuffle) ||
        !show_sort(SlowSort, 24, 2.4f, StandardShuffle) ||
        !show_sort(BogoSort, 5, 4.486f, StandardShuffle))
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
    initialize_procedural_audio();

    InitWindow(640, 480, "Sorting Visualizer");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(10, 10);
    SetTargetFPS(60);

    pthread_t sort_thread;
    pause = clock();
    pthread_create(&sort_thread, NULL, sort_proc, NULL);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        size_t array_runs = 1;
        for (size_t i = 1; i < sort_array->len; i++)
            if (sort_array->_arr[i] < sort_array->_arr[i - 1])
                array_runs++;
        draw_array(sort_array, GetScreenWidth() - 10, GetScreenHeight() - 10, 5, 5);
        DrawText(TextFormat("%s\nArray Accesses: %llu\n\t(%llu reads, %llu writes)\n%llu elements in array (%llu run{s})\nDelay: %.3fms",
                            status_text,
                            array_read_count + array_write_count,
                            array_read_count, array_write_count,
                            sort_array->len, array_runs,
                            array_access_delay),
                 10, 10, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();

    deinitialize_procedural_audio();
    CloseAudioDevice();

    pthread_kill(sort_thread, SIGTERM);
    Array_free(sort_array);

    MemFree(sort_array_read_indices);
    MemFree(sort_array_write_indices);

    return 0;
}
