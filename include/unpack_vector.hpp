#include <vector>
#include <iostream>
#include "is_stl_container.hpp"
#include "unpack_type_utils.hpp"
#include "tuple_utils.hpp"

namespace std
{

template <typename T>
class vector<unpack<T>> {
    private:
        using data_type = typename unpack_inversion<T>::type;
        using tuple_refs_type = typename unpack_tuple_refs_type<T>::type;
        using tuple_vec_iter_type = typename unpack_tuple_vec_iter_type<T>::type;
        data_type _data;
    public:
        vector<unpack<T>>() {}

        vector<unpack<T>>(const vector<unpack<T>>& v) {
            tuple_for_each(v._data, _data, [](auto& _old, auto& _new) {
                _new = _old;
            });
        }

        vector<unpack<T>>(vector<unpack<T>>&& v) {
            tuple_for_each(v._data, _data, [](auto&& _old, auto& _new) {
                _new = std::move(_old); 
            });
        }

        size_t size() const {
            return std::get<0>(_data).size();
        }

        size_t capacity() const {
            return std::get<0>(_data).capacity();
        }

        size_t max_size() const {
            return std::get<0>(_data).max_size();
        }

        void push_back(const T& elem) {
            tuple_for_each(elem, _data, [](auto& new_elem, auto& cur_vect) {
                cur_vect.push_back(new_elem);
            });
        }
        
        void resize(size_t size) {
            tuple_for_each(_data, [size](auto& cur_vect) { cur_vect.resize(size); });
        }

        tuple_refs_type operator[](size_t index) {
            return tuple_r_at_index(_data, index); 
        }

        ~vector() {}

        class iterator {
            public:
                using difference_type = std::ptrdiff_t;
                using value_type = T; 
                using pointer = tuple_refs_type*; // TODO
                using reference = tuple_refs_type;
                using iterator_category = std::bidirectional_iterator_tag;

                template <typename Func>
                iterator(data_type& d, Func&& f)
                    : _data(make_tuple_vec_iter(d, std::forward<Func>(f))) 
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
                
                iterator& operator--() {
                    tuple_for_each(_data, [](auto& iter) { iter--; }); 
                    return *this;     
                }

                iterator operator--(int) {
                    vector<unpack<T>>::iterator it = *this;
                    tuple_for_each(_data, [](auto& iter) { iter--; }); 
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
