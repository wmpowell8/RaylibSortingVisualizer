#include "../../Array.c"
#include "StandardShuffle.c"

static bool _me03mnepsmneov(Array array)
{
    Array copy = Array_copy(array);

    for (size_t i = 0; i < copy->len; i++)
    {
        float intermediate = 2 * (float)i / copy->len - 1;
        float square = intermediate * intermediate;
        Array_Result v = Array_at(copy, (square * square * intermediate + 1) / 2 * copy->len);
        Array_propagate_err(v);
        if (Array_set(array, i, v.value) == ARRAY_ERR)
            return false;
    }

    Array_free(copy);

    return StandardShuffle.fun(array);
}

/** Changes the array so that its values are based on a quintic function, then shuffles it using `StandardShuffle`. */
Algorithm QuinticShuffle = {_me03mnepsmneov, "Quintic Shuffle"};