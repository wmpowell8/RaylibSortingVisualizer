#include "../../Array.c"

static bool _aosososos(Array array)
{
    size_t ptr1 = 0;
    size_t ptr2 = array->len - 1;

    while (ptr2 > ptr1 + 1)
    {
        bool sorted = true;
        for (size_t i = ptr1; i < ptr2; i++)
        {
            Array_Result_Bool result = Array_reorder(array, i, i + 1); // TODO: OPTIMIZE
            Array_propagate_err(result);
            if (result.value == true)
                sorted = false;
        }
        if (sorted)
            return true;

        sorted = true;
        for (size_t i = ptr2; i > ptr1; i--)
        {
            Array_Result_Bool result = Array_reorder(array, i, i - 1); // TODO: OPTIMIZE
            Array_propagate_err(result);
            if (result.value == true)
                sorted = false;
        }
        if (sorted)
            return true;

        ptr1++;
        ptr2--;
    }

    return true;
}

Algorithm CocktailShakerSort = {_aosososos, "Cocktail Shaker Sort"};