// Credit to Stanley P. Y. Fung: https://arxiv.org/abs/2110.01111

#include "../../Array.c"

static bool _skskfccoff(Array array)
{
    for (int i = 0; i < array->len; i++)
        for (int j = 0; j < array->len; j++)
        {
            Array_Result at_i = Array_at(array, i);
            Array_propagate_err(at_i);
            Array_Result at_j = Array_at(array, j);
            Array_propagate_err(at_j);
            if (at_i.value < at_j.value)
            {
                if (Array_set(array, i, at_j.value) == ARRAY_ERR)
                    return false;
                if (Array_set(array, j, at_i.value) == ARRAY_ERR)
                    return false;
            }
        }
    return true;
}

Algorithm ICantBelieveItCanSort = {_skskfccoff, "I Can't Believe It Can Sort"};