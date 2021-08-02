#pragma once

#include "ExternalStorage.h"

#include <map>
#include <memory>
#include <stdexcept>
#include <limits>

template <typename key_type, typename value_type,
    template <typename key_storage_type, typename value_storage_type> typename storage_type>
class cached_map {
public:
    using element_type = std::pair< const key_type, value_type>;

    class iterator {
    public:
        iterator(cached_map* map) : cache(map) {}
        iterator(const key_type& key, cached_map* map) : key_ptr(std::make_unique<key_type>(key)), cache(map) {}

        /// <summary>
        /// Gets the iterator pointing to the next record in storage container.
        /// </summary>
        /// <returns></returns>
        iterator& operator++()
        {
            auto curr_key = *key_ptr.get();
            auto [next_key_defined, next_key] = cache->storage_.next_key(curr_key);
            if (next_key_defined)
            {
                key_ptr = std::make_unique<key_type>(next_key);
            }
            else
            {
                key_ptr = nullptr;
            }
            return *this;
        }

        /// <summary>
        /// Gets the keyvalue if the iterator can be dereferenced.
        /// Throws an exception if the iterator can not be dereferenced.
        /// </summary>
        /// <returns></returns>
        element_type& operator*()
        {
            if (key_ptr == nullptr) throw std::runtime_error("Trying to dereference nullptr");
            key_type key = *key_ptr.get();
            if (cache->is_cached(key))
            {
                cache->update_key_timestamp(key);
            }
            else
            {
                if (!cache->storage_.present(key)) throw std::runtime_error("Trying to dereference unknown key");

                auto [value_defined, value] = cache->storage_.read(key);
                if (!value_defined) throw std::runtime_error("Requested value is not defined");

                size_t value_size = value.size();
                if (value_size > cache->capacity_) throw std::runtime_error("Value size is bigger than cache capacity");

                element_type keyvalue = std::make_pair(key, value);
                cache->insert_to_cache(key, keyvalue, value_size);
            }
            auto keyvalue_ptr = (cache->elements_[key].second).get();
            return *keyvalue_ptr;
        }
        bool operator!=(const iterator& it)
        {
            return key_ptr != it.key_ptr;
        }
    private:
        /// <summary>
        /// The value is nullptr for end iterator.
        /// </summary>
        std::unique_ptr<key_type> key_ptr;

        /// <summary>
        /// Pointer to cached map that allows access to both cache and external storage.
        /// </summary>
        cached_map* cache;
    };

    using end_iterator = iterator;

    explicit cached_map(std::size_t mem_limit) : capacity_(mem_limit), size_(0), max_key_timestamp_(0) {}

    /// <summary>
    /// Gets the begin iterator to the external storage container.
    /// </summary>
    /// <returns></returns>
    iterator begin()
    {
        auto [key_defined, first_key] = storage_.first_key();
        if (key_defined)
        {
            return iterator(first_key, this);
        }
        return end();
    }

    /// <summary>
    /// Gets the end iterator to the external storage container.
    /// </summary>
    /// <returns></returns>
    end_iterator end()
    {
        return iterator(this);
    }

    /// <summary>
    /// Inserts new keyvalue if key does not exists.
    /// </summary>
    /// <param name="keyvalue"></param>
    /// <returns>Pair of iterator and bool value from which can be deduced
    /// whether the insertion took place or not.</returns>
    std::pair<iterator, bool> insert(const element_type& keyvalue)
    {
        auto& [key, value] = keyvalue;
        auto key_exists = storage_.present(key);
        storage_.write(key, value);
        bool inserted = false;

        if (!key_exists)
        {
            insert_to_cache(key, keyvalue, value.size());
            inserted = true;
        }

        return std::make_pair(iterator(key, this), inserted);

    }

    /// <summary>
    /// Inserts new keyvalue if key does not exists or assigns new
    /// value to the existing key.
    /// </summary>
    /// <param name="keyvalue"></param>
    /// <returns>Pair of iterator and bool value from which can be deduced
    /// whether the insertion took place or not.</returns>
    std::pair<iterator, bool> insert_or_assign(const element_type& keyvalue)
    {
        auto& [key, value] = keyvalue;
        auto key_exists = storage_.present(key);
        storage_.write(key, value);

        if (!key_exists)
        {
            insert_to_cache(key, keyvalue, value.size());
        }
        else
        {
            assign_new_value(key, value, keyvalue);
        }

        return std::make_pair(iterator(key, this), true);
    }

    /// <summary>
    /// Gets the iterator pointing to the given key.
    /// </summary>
    /// <param name="key"></param>
    /// <returns>Iterator attached to the requested key.
    /// Returns end iterator if key is not present in the storage.</returns>
    iterator find(const key_type& key)
    {
        if (storage_.present(key))
        {
            return iterator(key, this);
        }
        return end();
    }
private:
    using element_type_ptr = std::unique_ptr<element_type>;
    using key_timestamp = size_t;
    using key_element_type = std::pair<key_timestamp, element_type_ptr>;

    /// <summary>
    /// External storage reference.
    /// </summary>
    storage_type< key_type, value_type> storage_;

    /// <summary>
    /// Cache total capacity.
    /// </summary>
    std::size_t capacity_;

    /// <summary>
    /// Current size of the cache (sum of stored value sizes).
    /// </summary>
    std::size_t size_;

    /// <summary>
    /// Maximum value of currently stored key timestamp.
    /// </summary>
    std::size_t max_key_timestamp_;

    /// <summary>
    /// Container of cached keys with values.
    /// Key timestamps are stored inside element_type.
    /// </summary>
    std::map < key_type, key_element_type> elements_;

    /// <summary>
    /// Keys stored by their insertion / update order.
    /// Oldest key can be found at the first position.
    /// </summary>
    std::map<key_timestamp, key_type> keys_from_oldest_;

    bool is_cached(const key_type& key)
    {
        return elements_.find(key) != elements_.end();
    }

    /// <summary>
    /// Increments maximum key timestamp by 1.
    /// Checks if an increment does not cause overflow and if so,
    /// all key timestamps are updated to fill the holes between timestamp values
    /// (these holes might appear as a result of keys deletion from cache). 
    /// </summary>
    void increment_max_key_timestamp()
    {
        if (value_overflow(max_key_timestamp_))
        {
            std::map<key_timestamp, key_type> refractored_timestamps;
            max_key_timestamp_ = 0;
            for (auto it = keys_from_oldest_.begin(); it != keys_from_oldest_.end(); ++it)
            {
                max_key_timestamp_++;
                auto& [timestamp, key] = *it;
                refractored_timestamps.insert({ max_key_timestamp_, key });
                auto& element = *elements_.find(key);
                element.second.first = max_key_timestamp_;
            }
            keys_from_oldest_ = refractored_timestamps;
        }

        max_key_timestamp_++;
    }

    /// <summary>
    /// Updates given key as the newest cache record.
    /// </summary>
    /// <param name="key"></param>
    void update_key_timestamp(const key_type& key)
    {
        auto& element = *elements_.find(key);
        auto& curr_timestamp = element.second.first;
        if (curr_timestamp < max_key_timestamp_)
        {
            auto old_element_it = keys_from_oldest_.find(curr_timestamp);
            keys_from_oldest_.erase(old_element_it);
            increment_max_key_timestamp();
            curr_timestamp = max_key_timestamp_;
            keys_from_oldest_.insert({ curr_timestamp, element.first });
        }
    }

    /// <summary>
    /// Inserts keyvalue into cache if value size is smaller than cache capacity.
    /// </summary>
    /// <param name="key"></param>
    /// <param name="keyvalue">Pair of key and value for insertion to the cache container</param>
    /// <param name="value_size">Size of value placed in keyvalue</param>
    void insert_to_cache(const key_type& key, const element_type& keyvalue, size_t value_size)
    {
        if (value_size <= capacity_)
        {
            if (size_ + value_size > capacity_)
            {
                delete_old_elements(value_size);
            }

            increment_max_key_timestamp();

            size_ += value_size;

            keys_from_oldest_.insert({ max_key_timestamp_, key });
            elements_.insert(std::make_pair(key, key_element_type(max_key_timestamp_, std::make_unique<element_type>(keyvalue))));
        }
    }

    /// <summary>
    /// Assigns new value into cache.
    /// If keyvalue is stored in cache already, value is updated.
    /// If keyvalue is not stored in cache, the whole keyvalue is inserted if possible.
    /// </summary>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <param name="keyvalue">Updated pair of key and value to value assignment</param>
    void assign_new_value(const key_type& key, const value_type& value, const element_type& keyvalue)
    {
        auto value_size = value.size();
        if (is_cached(key))
        {
            if (value_size > capacity_)
            {
                update_key_timestamp(key);
            }
            else
            {
                key_element_type& curr_element = elements_[key];
                element_type_ptr& curr_value_ptr = curr_element.second;
                size_t old_value_size = (*curr_value_ptr.get()).second.size();
                size_ = size_ - old_value_size + value_size;
                update_key_timestamp(key);
                if (size_ > capacity_)
                {
                    delete_old_elements(size_ - capacity_);
                }
                curr_value_ptr = std::make_unique<element_type>(keyvalue);
            }
        }
        else
        {
            insert_to_cache(key, keyvalue, value_size);
        }
    }

    /// <summary>
    /// Frees required capacity by deleting as many old cache records as needed. 
    /// </summary>
    /// <param name="required_capacity">Size of cache space that needs to be freed</param>
    void delete_old_elements(size_t required_capacity)
    {
        size_t freed_capacity = 0;
        while (size_ > 0 && freed_capacity < required_capacity)
        {
            auto oldest_key_order_it = keys_from_oldest_.begin();
            auto oldest_key = (*oldest_key_order_it).second;
            auto oldest_key_it = elements_.find(oldest_key);
            key_element_type& element = (*oldest_key_it).second;

            auto& [key, value] = *(element.second.get());
            auto value_size = value.size();
            size_ -= value_size;
            freed_capacity += value_size;

            elements_.erase(oldest_key_it);
            keys_from_oldest_.erase(oldest_key_order_it);
        }
    }

    /// <summary>
    /// Checks if increment by 1 would cause value overflow.
    /// </summary>
    /// <typeparam name="T">Numeric type</typeparam>
    /// <param name="curr_value">Value to be checked against arithmetic overflow</param>
    /// <returns>Returns true if overflow would occur, false otherwise</returns>
    template<typename T>
    bool value_overflow(T curr_value)
    {
        return curr_value >= std::numeric_limits<T>::max() - 1;
    }
};