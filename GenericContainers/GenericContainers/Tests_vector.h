#pragma once

#include <vector>

#include "Vector.h"

class tests_vector {
public:
    template<typename item_type>
    static bool vector_content_same(const vector<item_type>& v1, const std::vector<item_type>& v2);

    template<typename item_type>
    static void test_vector(int ini_size, const item_type& ini_value,
        const item_type& push_value,
        int resize_size, const item_type& resize_value,
        int index_to_modify, const item_type& new_value);

    static void test_int_vector();
    static void test_double_vector();
    static void test_string_vector();
};