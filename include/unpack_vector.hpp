#include <vector>
#include <iostream>
#include "is_stl_container.hpp"
#include "unpack_inversion.hpp"
#include "tuple_for_each.hpp"

#define VECTOR_INIT_CAPACITY 64

namespace std
{

template <typename T>
class vector<unpack<T>> {
    private:
        using data_type = typename unpack_inversion<T>::type;
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
        ~vector() {
            delete [] _data;
        }
};

}
