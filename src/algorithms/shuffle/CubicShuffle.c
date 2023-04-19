#include "../../Array.c"
#include "StandardShuffle.c"

static bool _kr83klfdkjmdpw(Array array)
{
    Array copy = Array_copy(array);
    if (copy == NULL)
        return false;

    for (size_t i = 0; i < copy->len; i++)
    {
        float intermediate = 2 * (float)i / copy->len - 1;
        Array_Result v = Array_at(copy, (intermediate * intermediate * intermediate + 1) / 2 * copy->len);
        Array_propagate_err(v);
        if (Array_set(array, i, v.value) == ARRAY_ERR)
            return false;
    }

    Array_free(copy);

    return StandardShuffle.fun(array);
}

/** Changes the array so that its values are based on a cubic function, then shuffles it using `StandardShuffle`. */
Algorithm CubicShuffle = {_kr83klfdkjmdpw, "Cubic Shuffle"};