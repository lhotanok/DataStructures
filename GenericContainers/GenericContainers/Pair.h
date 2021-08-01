#pragma once

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