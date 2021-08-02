#include "Tests_vector.h"

template<typename item_type>
bool tests_vector::vector_content_same(const vector<item_type>& v1, const std::vector<item_type>& v2) {
    for (size_t i = 0; i < v2.size(); i++)
    {
        if (v1[i] != v2[i])
        {
            return false;
        }
    }

    return true;
}

template<typename item_type>
void tests_vector::test_vector(int ini_size, const item_type& ini_value,
    const item_type& push_value,
    int resize_size, const item_type& resize_value,
    int index_to_modify, const item_type& new_value) {

    vector<item_type> v1(ini_size, ini_value);
    std::vector<item_type> v2(ini_size, ini_value);

    std::cout << "Test passed: " << vector_content_same(v1, v2) << ". Vector content: "; v1.print();

    v1.push_back(push_value);
    v2.push_back(push_value);
    std::cout << "Test passed: " << vector_content_same(v1, v2) << ". Vector content: "; v1.print();

    v1.resize(resize_size, resize_value);
    v2.resize(resize_size, resize_value);
    std::cout << "Test passed: " << vector_content_same(v1, v2) << ". Vector content: "; v1.print();

    v1[index_to_modify] = new_value;
    v2[index_to_modify] = new_value;
    std::cout << "Test passed: " << vector_content_same(v1, v2) << ". Vector content: "; v1.print();

    std::cout << std::endl;
}

void tests_vector::test_int_vector() {
    test_vector<int>(4, 6, 1, 10, 7, 8, 10);
}

void tests_vector::test_double_vector() {
    test_vector<double>(4, 6.9, 1.18, 10, 7, 6, 10.45);
}

void tests_vector::test_string_vector() {
    test_vector<const char*>(4, "new", "item\t", 10, "resized", 7, "changed");
}