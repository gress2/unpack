#ifndef UNPACK_ITERATOR
#define UNPACK_ITERATOR

#include "unpack_details.hpp"

template <typename tuple_iters_type>
class unpack_iterator {
    private: 
        using underlying_data_type = typename remove_iter_type<tuple_iters_type>::type;
        using tuple_refs_type = typename unpack_tuple_refs_type<underlying_data_type>::type;
        tuple_iters_type _data;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = tuple_refs_type; 
        using pointer = tuple_refs_type; // TODO
        using reference = tuple_refs_type;
        using iterator_category = std::bidirectional_iterator_tag;

        unpack_iterator(tuple_iters_type&& d) 
            : _data(std::forward<tuple_iters_type>(d))
        {
        }
        
        unpack_iterator& operator++() {
            tuple_for_each([](auto& iter) { iter++; }, _data); 
            return *this;     
        }

        unpack_iterator operator++(int) {
            auto it = *this;
            tuple_for_each([](auto& iter) { iter++; }, _data); 
            return it;    
        } 
        
        unpack_iterator& operator--() {
            tuple_for_each([](auto& iter) { iter--; }, _data); 
            return *this;     
        }

        unpack_iterator operator--(int) {
            auto it = *this;
            tuple_for_each([](auto& iter) { iter--; }, _data); 
            return it;    
        }

        tuple_refs_type operator*() {
            return make_tuple_refs(_data); 
        }

        bool operator==(const unpack_iterator& rhs) {
            return rhs._data == _data;
        }

        bool operator!=(const unpack_iterator& rhs) {
            return rhs._data != _data;       
        }

        tuple_iters_type* data() {
            return &_data;
        }
};

template <typename tuple_iters_type>
class unpack_const_iterator {
    private:
        using underlying_data_type = typename remove_iter_type<tuple_iters_type>::type;
        using tuple_const_refs_type = typename unpack_tuple_const_refs_type<underlying_data_type>::type;
        tuple_iters_type _data;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = tuple_const_refs_type;
        using pointer = tuple_const_refs_type; // TODO
        using reference = tuple_const_refs_type;
        using iterator_category = std::bidirectional_iterator_tag;

    unpack_const_iterator(tuple_iters_type&& d)
        : _data(std::forward<tuple_iters_type>(d)) 
    {
    }

    unpack_const_iterator& operator++() {
        tuple_for_each([](auto& iter) { iter++; }, _data);
        return *this;     
    }

    unpack_const_iterator operator++(int) {
        auto it = *this;
        tuple_for_each([](auto& iter) { iter++; }, _data); 
        return it;    
    } 
    
    unpack_const_iterator& operator--() {
        tuple_for_each([](auto& iter) { iter--; }, _data); 
        return *this;     
    }

    unpack_const_iterator operator--(int) {
        auto it = *this;
        tuple_for_each([](auto& iter) { iter--; }, _data); 
        return it;    
    }

    tuple_const_refs_type operator*() {
        return make_tuple_refs(_data); 
    }

    bool operator==(const unpack_const_iterator& rhs) {
        return rhs._data == _data;
    }

    bool operator!=(const unpack_const_iterator& rhs) {
        return rhs._data != _data;       
    }
   
    tuple_iters_type* data() {
        return &_data;
    }
};

#endif
