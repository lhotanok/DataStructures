#include "Tests_vector.h"
#include "Tests_pair.h"

int main()
{
    std::cout << std::boolalpha;

    tests_vector::test_int_vector();
    tests_vector::test_double_vector();
    tests_vector::test_string_vector();

    tests_pair::test_int_pair();
    tests_pair::test_string_pair();
    tests_pair::test_mixed_type_pair();

    tests_pair::test_ctors();
}