#include "../../Array.c"

static bool _wlfc8en(Array array)
{
    unsigned char sorted = 0;
    while (true)
    {
        sorted |= 1;
        for (size_t i = 1; i < array->len - 1; i += 2)
        {
            Array_Result_Bool result = Array_reorder(array, i, i+1);
            Array_propagate_err(result);
            if (result.value)
                sorted = 0;
        }
        if (sorted == 3)
            return true;
        sorted |= 2;
        for (size_t i = 0; i < array->len - 1; i += 2)
        {
            Array_Result_Bool result = Array_reorder(array, i, i+1);
            Array_propagate_err(result);
            if (result.value)
                sorted = 0;
        }
        if (sorted == 3)
            return true;
    }
    return false;
}

Algorithm OddEvenSort = {_wlfc8en, "Odd-Even Sort"};