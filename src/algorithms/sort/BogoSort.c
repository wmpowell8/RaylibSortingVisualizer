#include "../../Array.c"
#include "../../rlonce.h"

static bool _fnenofeiwfeoi(Array array)
{
    while (true)
    {
        // Return true if array is sorted

        bool sorted = true;
        Array_Result first = Array_at(array, 0);
        Array_propagate_err(first);
        unsigned int previous = first.value;
        for (size_t i = 1; i < array->len; i++)
        {
            Array_Result current = Array_at(array, i);
            Array_propagate_err(current);
            if (current.value >= previous)
                previous = current.value;
            else
            {
                sorted = false;
                break;
            }
        }
        if (sorted)
            return true;

        // Shuffle array

        for (size_t i = 0; i < array->len - 1; i++)
            if (Array_swap(array, i, GetRandomValue(i, array->len - 1)) == ARRAY_ERR)
                return false;
    }
}

Algorithm BogoSort = {_fnenofeiwfeoi, "Bogosort"};