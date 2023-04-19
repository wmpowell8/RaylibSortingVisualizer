#include "../../Array.c"

static bool _39pqohnfpqbv9ioeopd(Array array)
{
    // array accesses to make cool animation of reading all elements (not required)
    for (size_t i = 0; i < array->len; i++)
        if (Array_at(array, i).condition == ARRAY_ERR)
            return false;
    
    return true;
}

Algorithm NoShuffle = {_39pqohnfpqbv9ioeopd, "No Shuffle"};