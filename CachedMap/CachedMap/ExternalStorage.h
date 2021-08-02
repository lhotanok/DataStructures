#pragma once

#include <map>
#include <algorithm>

template <typename key_storage_t, typename value_storage_t>
class external_storage {
public:
    std::size_t size(const key_storage_t& key) const
    {
        size_t value_size;
        if (present(key))
        {
            value_size = elements[key].size();
        }
        return value_size;
    }

    void write(const key_storage_t& key, const value_storage_t& value)
    {
        elements.insert_or_assign(key, value);
    }

    std::tuple< bool, value_storage_t> read(const key_storage_t& key)
    {
        auto value_it = elements.find(key);
        if (value_it != elements.end())
        {
            return std::make_tuple(true, (*value_it).second);
        }
        return std::make_tuple(false, value_storage_t());
    }

    std::tuple< bool, key_storage_t> first_key()
    {
        auto first_it = elements.begin();
        if (first_it != elements.end())
        {
            key_storage_t key = (*first_it).first;
            return std::make_tuple(true, key);
        }
        return std::make_tuple(false, key_storage_t());
    }

    std::tuple< bool, key_storage_t> next_key(const key_storage_t& key)
    {
        auto curr_iterator = elements.find(key);
        auto next_iterator = ++curr_iterator;
        if (next_iterator != elements.end())
        {
            return std::make_tuple(true, (*next_iterator).first);
        }
        return std::make_tuple(false, key_storage_t());
    }

    bool present(const key_storage_t& key)
    {
        return elements.find(key) != elements.end();
    }

private:
    std::map<key_storage_t, value_storage_t> elements;
};
