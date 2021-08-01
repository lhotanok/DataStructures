#pragma once

#include <iostream>
#include <memory>

template<typename item>
class vector {
private:
    size_t size_m, capacity_m;
    std::unique_ptr<item[]> arr_m;

    void fill_with_values(item value, size_t lower_bound, size_t upper_bound);
public:
    vector(size_t size, item value);

    size_t size() const;
    size_t capacity() const;

    void reserve(size_t new_cap);
    void resize(size_t count, item value);

    void push_back(item value);

    item& operator [] (size_t pos);

    void print() const;
};

#include "Vector.impl.h"