#include "../../Array.c"

static bool _sisisisidndndn(Array array)
{
    for (size_t i = 1; i < array->len; i++)
        for (size_t j = i; j > 0; j--)
        {
            Array_Result_Bool res = Array_reorder(array, j - 1, j);
            Array_propagate_err(res);
            if (!res.value)
                break;
        }
    return true;
}

Algorithm InsertionSort = {_sisisisidndndn, "Insertion Sort"};