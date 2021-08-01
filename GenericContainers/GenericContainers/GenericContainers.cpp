#include <iostream>

#include "Vector.h"
#include "Pair.h"

class test {
public:
    static void test_int_vector() {
        vector<int> v1(4, 6);
        v1.print();
        v1.push_back(1);
        v1.print();
        v1.resize(10, 7);
        v1.print();
        v1[8] = 10;
        v1.print();
        std::cout << std::endl;
    }
    static void test_double_vector() {
        vector<double> v2(4, 6.9);
        v2.print();
        v2.push_back(1.18);
        v2.print();
        v2.resize(10, 7);
        v2.print();
        v2[8] = 10.45;
        v2.print();
        std::cout << std::endl;
    }
    static void test_string_vector() {
        vector<std::string> v3(4, "new");
        v3.print();
        v3.push_back("item\t");
        v3.print();
        v3.resize(10, "resized");
        v3.print();
        v3[8] = "changed";
        v3.print();
        std::cout << std::endl;
    }
    static void test_int_pair() {
        auto p = pair<int, int>::make_pair(9, 6);
        p.print();
    }
    static void test_string_pair() {
        auto p = pair<std::string, std::string>::make_pair("Hello", "world");
        p.print();
    }
    static void test_mixed_type_pair() {
        auto p = pair<std::string, int>::make_pair("Mixed", -60);
        p.print();
    }
    static void test_make_pair_method() {
        auto p = pair<int, std::string>::make_pair(1, "sec");
        p.print();
        auto first = p.first;
        auto second = p.second;
        std::cout << "First item: " << first << "\tSecond item: " << second << std::endl;
        test_pair_copy_ctor(p);
        test_pair_move_ctor(std::move(p));
    }
    template<typename first_type, typename second_type>
    static void test_pair_copy_ctor(const pair<first_type, second_type>& p) {
        auto copy_pair = pair<decltype(p.first), decltype(p.second)>(p);
        std::cout << "Copy of pair created: ";
        copy_pair.print();
    }

    template<typename first_type, typename second_type>
    static void test_pair_move_ctor(pair<first_type, second_type>&& pair) {
        auto move_pair = std::move(pair);
        std::cout << "pair moved: ";
        move_pair.print();
    }
};

int main()
{
    test::test_int_vector();
    test::test_double_vector();
    test::test_string_vector();
    test::test_int_pair();
    test::test_string_pair();
    test::test_mixed_type_pair();
    test::test_make_pair_method();
}