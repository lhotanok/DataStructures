#include "Tests_pair.h"

template<typename first_type, typename second_type>
bool tests_pair::pair_content_same(const pair<first_type, second_type>& p1, const std::pair<first_type, second_type>& p2)
{
    return p1.first == p2.first && p1.second == p2.second;
}

template<typename first_type, typename second_type>
void tests_pair::test_pair(const first_type& first, const second_type& second) {
    auto p1 = pair<first_type, second_type>::make_pair(first, second);
    auto p2 = std::make_pair(first, second);

    std::cout << "Test passed: " << pair_content_same(p1, p2) << ". Pair content: "; p1.print();
}

void tests_pair::test_int_pair() {
    test_pair<int, int>(9, 6);
}

void tests_pair::test_string_pair() {
    test_pair<std::string, std::string>("Hello", "world");
}

void tests_pair::test_mixed_type_pair() {
    test_pair<std::string, int>("Mixed", -60);
}

void tests_pair::test_ctors() {
    auto p = pair<int, std::string>::make_pair(1, "sec");

    test_pair_copy_ctor(p);
    test_pair_move_ctor(std::move(p));
}

template<typename first_type, typename second_type>
void tests_pair::test_pair_copy_ctor(const pair<first_type, second_type>& p) {
    auto old_pair = make_pair(p.first, p.second);
    auto copy_pair = pair<decltype(p.first), decltype(p.second)>(p);

    std::cout << "Test passed: " << pair_content_same(copy_pair, old_pair);
    std::cout << ". Copy of pair created: ";
    copy_pair.print();
}

template<typename first_type, typename second_type>
void tests_pair::test_pair_move_ctor(pair<first_type, second_type>&& p) {
    auto old_pair = make_pair(p.first, p.second);
    auto move_pair = std::move(p);

    std::cout << "Test passed: " << pair_content_same(move_pair, old_pair);
    std::cout << ". Pair moved: ";
    move_pair.print();
}