#include "../../Array.c"

static bool _ms(Array array, size_t start, size_t end)
{
    if (end <= start + 1)
        return true;

    size_t mid = (start + end) >> 1;

    if (!_ms(array, start, mid))
        return false;
    if (!_ms(array, mid, end))
        return false;

    size_t ptr1 = start;
    Array_Result value1 = Array_at(array, ptr1);
    Array_propagate_err(value1);
    size_t ptr2 = mid;
    Array_Result value2 = Array_at(array, ptr2);
    Array_propagate_err(value2);

    Array full = Array_new(end - start);
    size_t full_ptr = 0;
    while (full_ptr < full->len)
    {
        Array_Result *value;
        size_t *ptr;

        if (ptr1 < mid && (ptr2 >= end || value1.value < value2.value))
        {
            value = &value1;
            ptr = &ptr1;
        }
        else
        {
            value = &value2;
            ptr = &ptr2;
        }
        Array_propagate_err((*value));

        if (Array_set(full, full_ptr, value->value) == ARRAY_ERR)
            return false;
        full_ptr++;
        (*ptr)++;
        *value = Array_at(array, *ptr);
    }
    for (size_t i = 0; i < full->len; i++)
    {
        Array_Result value = Array_at(full, i);
        Array_propagate_err(value);
        if (Array_set(array, start + i, value.value) == ARRAY_ERR)
            return false;
    }
    Array_free(full);

    return true;
}

static bool _pdkowdwpko(Array array)
{
    if (array->len <= 0)
        return true;
    return _ms(array, 0, array->len);
}

Algorithm MergeSort = {_pdkowdwpko, "Merge Sort"};