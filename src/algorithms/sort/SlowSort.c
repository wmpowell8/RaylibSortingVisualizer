#include "../../Array.c"

static bool _slow_sort(Array array, size_t start, size_t end)
{
    if (start >= end)
        return true;
    size_t middle = (start + end) >> 1;
    if (!_slow_sort(array, start, middle))
        return false;
    if (!_slow_sort(array, middle + 1, end))
        return false;
    Array_propagate_err(Array_reorder(array, middle, end));
    if (!_slow_sort(array, start, end - 1))
        return false;
    return true;
}

static bool _whfoen3od7(Array array)
{
    return _slow_sort(array, 0, array->len - 1);
}

Algorithm SlowSort = {_whfoen3od7, "Slow Sort"};