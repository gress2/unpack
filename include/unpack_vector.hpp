#include <vector>
#include <iostream>
#include "is_stl_container.hpp"
#include "unpack_type_utils.hpp"
#include "tuple_utils.hpp"

#define VECTOR_INIT_CAPACITY 64

template <typename T>
void print() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

namespace std
{

template <typename T>
class vector<unpack<T>> {
    private:
        using data_type = typename unpack_inversion<T>::type;
        using tuple_refs_type = typename unpack_tuple_refs_type<T>::type;
        using tuple_vec_iter_type = typename unpack_tuple_vec_iter_type<T>::type;
        data_type* _data;
        size_t _size, _capacity;
        void double_capacity() {
            _capacity *= 2;
            _data = (data_type*)realloc(_data, sizeof(data_type) * _capacity);
        }
    public:
        vector<unpack<T>>() {
            _size = 0;
            _capacity = VECTOR_INIT_CAPACITY;
            _data = new data_type[_capacity]; 
        }
        vector<unpack<T>>(const vector<unpack<T>>& v) {
            // copy constructor
        }
        vector<unpack<T>>(vector<unpack<T>>&& v) {
            // move constructor
        }
        size_t size() const {
            return _size;
        }
        void push_back(const T& elem) {
            if (_size + sizeof(data_type) >= _capacity) {
                double_capacity();
            }         

            tuple_for_each(elem, *_data, [](auto& x, auto& y) {
                y.push_back(x);
            });
            
            _size++;
        }

        tuple_refs_type operator[](size_t index) {
            return tuple_r_at_index(*_data, index); 
        }

        ~vector() {
            delete [] _data;
        }

        class iterator {
            public:
                using difference_type = std::ptrdiff_t;
                using value_type = T; 
                using pointer = tuple_refs_type*;
                using reference = tuple_refs_type;
                using iterator_category = std::bidirectional_iterator_tag;
                template <typename Func>
                iterator(data_type* d, Func&& f)
                    : _data(make_tuple_vec_iter(*d, std::forward<Func>(f))) 
                {
                }
                iterator& operator++() {
                    tuple_for_each(_data, [](auto& iter) { iter++; }); 
                    return *this;     
                }
                iterator operator++(int) {
                    vector<unpack<T>>::iterator it = *this;
                    tuple_for_each(_data, [](auto& iter) { iter++; }); 
                    return it;    
                } 
                tuple_refs_type operator*() {
                    return make_tuple_refs(_data); 
                }
                bool operator==(const iterator& rhs) {
                    return rhs._data == _data;
                }
                bool operator!=(const iterator& rhs) {
                    return rhs._data != _data;       
                }
                template <typename DistType>
                friend DistType distance(const iterator& begin, const iterator& end) {
                   return std::distance(std::get<0>(end), std::get<0>(begin));
                }
            private:
                tuple_vec_iter_type _data;
        };
        iterator begin() {
            return iterator(_data, [](auto& iter) { return iter.begin(); }); 
        }
        iterator end() {
            return iterator(_data, [](auto& iter) { return iter.end(); });
        }
};

}
