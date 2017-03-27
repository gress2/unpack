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
        using ref_type = typename unpack_ref_type<T>::type;
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

        ref_type operator[](size_t index) {
            return tuple_r_at_index(*_data, index); 
        }

        ~vector() {
            delete [] _data;
        }

        class iterator {
            public:
                using difference_type = std::ptrdiff_t;
                using value_type = ref_type;
                using pointer = ref_type*;
                using reference = ref_type&;  
                using iterator_category = std::bidirectional_iterator_tag;
                iterator(data_type* d, size_t pos)
                    : _data(d), 
                      _pos(pos), 
                      _target(tuple_r_at_index(*d, pos)),
                      _ptr(&_target) 
                {
                }
                iterator& operator++() {
                    _pos++;
                    return *this;     
                }
                iterator operator++(int) {
                    vector<unpack<T>>::iterator it = *this;
                    _pos++;
                    return it;    
                } 
                ref_type* operator->() {
                    update_target();
                    return _ptr;
                }
                ref_type& operator*() {
                    update_target(); 
                    return *_ptr;
                }
                bool operator==(const iterator& rhs) {
                    return ((rhs._data == _data) && (rhs._pos == _pos));                    
                }
                bool operator!=(const iterator& rhs) {
                    return !((rhs._data == _data) && (rhs._pos == _pos));                    
                }
                template <typename DistType>
                friend DistType distance(const iterator& begin, const iterator& end) {
                   return end._pos - begin._pos; 
                }
            private:
                data_type* _data;
                ref_type* _ptr;
                ref_type _target;
                size_t _pos;
                void update_target() {
                    _target = tuple_r_at_index(*_data, _pos);
                }
        };
        iterator begin() {
            return iterator(_data, 0); 
        }
        iterator end() {
            return iterator(_data, _size + 1);
        }
};

}
