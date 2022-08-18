#include "../../Array.c"

static bool _efjnefnoef(Array array)
{
    for (size_t i = array->len - 1; i > 0; i--)
    {
        bool sorted = true;
        for (size_t j = 0; j < i; j++)
        {
            Array_Result_Bool result = Array_reorder(array, j, j + 1); // TODO: OPTIMIZE
            Array_propagate_err(result);
            if (result.value == true)
                sorted = false;
        }
        if (sorted)
            return true;
    }
    return true;
}

Algorithm BubbleSort = {_efjnefnoef, "Bubble Sort"};