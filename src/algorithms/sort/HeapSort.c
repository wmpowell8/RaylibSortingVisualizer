#include "../../Array.c"

static bool _sift_down(Array array, size_t starting_idx, size_t heap_size)
{
    if (heap_size > array->len)
        return false;
    size_t parent_idx = starting_idx;
    
    while (true)
    {
        size_t child1idx = (parent_idx << 1) + 1;
        if (child1idx >= heap_size)
            break;
        Array_Result parent_value = Array_at(array, parent_idx);
        Array_propagate_err(parent_value);
        Array_Result child1value = Array_at(array, child1idx);
        Array_propagate_err(child1value);
        size_t child2idx = child1idx + 1;
        if (child2idx >= heap_size)
        {
            if (parent_value.value > child1value.value) break;

            if (Array_set(array, parent_idx, child1value.value) == ARRAY_ERR)
                return false;
            if (Array_set(array, child1idx, parent_value.value) == ARRAY_ERR)
                return false;
            parent_idx = child1idx;
            continue;
        }
        Array_Result child2value = Array_at(array, child2idx);
        Array_propagate_err(child2value);
        size_t bigger_idx;
        unsigned int bigger_value;
        if (child1value.value > child2value.value)
        {
            bigger_idx = child1idx;
            bigger_value = child1value.value;
        }
        else
        {
            bigger_idx = child2idx;
            bigger_value = child2value.value;
        }
        if (parent_value.value > bigger_value)
            break;
        if (Array_set(array, parent_idx, bigger_value) == ARRAY_ERR)
            return false;
        if (Array_set(array, bigger_idx, parent_value.value) == ARRAY_ERR)
            return false;
        parent_idx = bigger_idx;
    }

    return true;
}

static bool _b3ifhwkdh(Array array)
{
    if (array->len <= 0)
        return true;

    for (size_t i = (array->len >> 1) - 1; i != -1; i--)
        if (!_sift_down(array, i, array->len))
            return false;
    for (size_t i = array->len - 1; i > 1; i--)
    {
        Array_swap(array, 0, i);
        if (!_sift_down(array, 0, i))
            return false;       
    }
    Array_propagate_err(Array_reorder(array, 0, 1));
    return true;
}

Algorithm HeapSort = {_b3ifhwkdh, "Heap Sort"};