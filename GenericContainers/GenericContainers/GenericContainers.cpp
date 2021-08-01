#include <iostream>
#include <memory>

//using namespace std;

template<typename item>
class vector {
private:
    size_t size_m, capacity_m;
    std::unique_ptr<item[]> arr_m;

    void fill_with_values(item value, size_t lower_bound, size_t upper_bound)
    {
        for (size_t i = lower_bound; i < upper_bound; i++)
        {
            arr_m[i] = value;
        }
    }
public:
    vector(size_t size, item value) 
        : size_m(size), capacity_m(size), arr_m(std::make_unique<item[]>(size))
    {
        fill_with_values(value, 0, size);
    }

    item size() const
    {
        return size_m;
    }

    item capacity() const
    {
        return capacity_m;
    }

    void reserve(size_t new_cap)
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

    void resize(size_t count, item value)
    {
        if (count > size_m)
        {
            reserve(count);
            fill_with_values(value, size_m, capacity_m);
            size_m = capacity_m;
        }
    }

    void push_back(item value)
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

    item& operator [] (size_t pos)
    {
        return arr_m[pos];
    }

    void print() const
    {
        for (size_t i = 0; i < size_m; i++)
        {
            std::cout << arr_m[i] << ' ';
        }
        std::cout << std::endl;
    }
};

template<typename first_type, typename second_type>
class pair {
private:
    pair(first_type first, second_type second)
        : first(first), second(second) {} // ctor
public:
    first_type first;
    second_type second;

    pair(const pair& pair) : first(pair.first), second(pair.second) {} // copy-ctor
    pair(pair&& pair) noexcept : first(pair.first), second(pair.second) {} // move-ctor
    ~pair() = default; // destructor

    static auto make_pair(first_type first, second_type second) {
        return pair<first_type, second_type>(first, second);
    }

    void print() const
    {
        std::cout << '<' << first << ", " << second << '>' << std::endl;
    }
};

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