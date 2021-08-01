#pragma once

#include "Vector.h"

template<typename item>
void vector<item>::fill_with_values(item value, size_t lower_bound, size_t upper_bound)
{
    for (size_t i = lower_bound; i < upper_bound; i++)
    {
        arr_m[i] = value;
    }
}

template<typename item>
vector<item>::vector(size_t size, item value)
    : size_m(size), capacity_m(size), arr_m(std::make_unique<item[]>(size))
{
    fill_with_values(value, 0, size);
}

template<typename item>
size_t vector<item>::size() const
{
    return size_m;
}

template<typename item>
size_t vector<item>::capacity() const
{
    return capacity_m;
}

template<typename item>
void vector<item>::reserve(size_t new_cap)
{
    if (new_cap > capacity_m)
    {
        std::unique_ptr<item[]> new_arr = std::make_unique<item[]>(new_cap);
        capacity_m = new_cap;
        for (size_t i = 0; i < size_m; i++)
        {
            new_arr[i] = arr_m[i];
        }
        arr_m = move(new_arr);
    }
}

template<typename item>
void vector<item>::resize(size_t count, item value)
{
    if (count > size_m)
    {
        reserve(count);
        fill_with_values(value, size_m, capacity_m);
        size_m = capacity_m;
    }
}

template<typename item>
void vector<item>::push_back(item value)
{
    if (capacity_m < size_m + 1)
    {
        // to guarantee amortized constant time complexity
        size_t new_size = 2 * size_m;
        reserve(new_size);
    }
    arr_m[size_m] = value;
    size_m++;
}

template<typename item>
item& vector<item>::operator [] (size_t pos)
{
    return arr_m[pos];
}

template<typename item>
void vector<item>::print() const
{
    for (size_t i = 0; i < size_m; i++)
    {
        std::cout << arr_m[i] << ' ';
    }

    std::cout << std::endl;
}