#include "../../Array.c"

static bool _qs(Array array, size_t start, size_t end)
{
    if (end + 1 <= start + 1) // adding 1 first because of edge case when start == 0 and end == ULONG_MAX
        return true;

    Array_Result pivot = Array_at(array, start);
    Array_propagate_err(pivot);
    size_t ptr1 = start;
    size_t ptr2 = end;
    while (ptr1 + 1 <= ptr2 + 1) // adding 1 first because of edge case when ptr1 == 0
    {
        Array_Result value1 = Array_at(array, ptr1);
        Array_propagate_err(value1);
        if (value1.value >= pivot.value)
        {
            Array_Result value2 = Array_at(array, ptr2);
            Array_propagate_err(value2);
            if (Array_set(array, ptr1, value2.value) == ARRAY_ERR)
                return false;
            if (Array_set(array, ptr2, value1.value) == ARRAY_ERR)
                return false;
            ptr2--;
        }
        else
        {
            ptr1++;
        }
    }
    if (!_qs(array, start, ptr2))
        return false;
    if (!_qs(array, ptr1, end))
        return false;
    return true;
}

static bool _kargargar(Array array)
{
    if (array->len <= 0)
        return true;
    return _qs(array, 0, array->len - 1);
}

Algorithm QuickSort = {_kargargar, "Quicksort"};