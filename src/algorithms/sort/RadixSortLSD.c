#include "../../Array.c"

static bool _28djw9fjw(Array array)
{
    Array buckets[4];
    size_t bucket_size = 0;

    while (true)
    {
        for (size_t i = 0; i < 4; i++)
            buckets[i] = Array_new(0);
        
        for (size_t i = 0; i < array->len; i++)
        {
            Array_Result value = Array_at(array, i);
            Array_propagate_err(value);
            if (Array_push(buckets[(value.value >> bucket_size) & 3], value.value) == ARRAY_ERR)
                return false;
        }

        size_t reconstruct_index = 0;
        bool sorted = true;
        unsigned int previous_item = 0;
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < buckets[i]->len; j++)
            {
                Array_Result value = Array_at(buckets[i], j);
                Array_propagate_err(value);
                if (Array_set(array, reconstruct_index, value.value) == ARRAY_ERR)
                    return false;
                reconstruct_index++;
                if (value.value < previous_item)
                    sorted = false;
                previous_item = value.value;
            }

        for (size_t i = 0; i < 4; i++)
            Array_free(buckets[i]);

        if (sorted)
            return true;
        
        bucket_size += 2;
    }
    return false;
}

Algorithm RadixSortLSD = {_28djw9fjw, "Radix Sort (LSD)"};