#pragma once

#include <iostream>
#include <string>

#include "Pair.h"

class tests_pair {
public:
    template<typename first_type, typename second_type>
    static bool pair_content_same(const pair<first_type, second_type>& p1, const std::pair<first_type, second_type>& p2);

    template<typename first_type, typename second_type>
    static void test_pair(const first_type &first, const second_type &second);
    
    static void test_int_pair();
    static void test_string_pair();
    static void test_mixed_type_pair();

    static void test_ctors();

    template<typename first_type, typename second_type>
    static void test_pair_copy_ctor(const pair<first_type, second_type>& p);

    template<typename first_type, typename second_type>
    static void test_pair_move_ctor(pair<first_type, second_type>&& pair);
};