#include <vector> 
#include <iostream>
#include "unpack_type_utils.hpp"
#include "tuple_utils.hpp"

namespace std
{

template <typename T>
class vector<unpack<T>> {
    private:
        using data_type = typename unpack_inversion<T>::type;
        using tuple_refs_type = typename unpack_tuple_refs_type<T>::type;
        using tuple_const_refs_type = typename unpack_tuple_const_refs_type<T>::type;
        using tuple_vec_iter_type = typename unpack_tuple_vec_iter_type<T>::type;
        using tuple_vec_const_iter_type = typename unpack_tuple_vec_const_iter_type<T>::type;
        data_type _data;
        
        void throw_if_out_of_bounds(size_t index) const {
            if (index >= this->size()) {
                std::string error = "vector::at: __n (which is " + std::to_string(index) + 
                    ") >= this->size() (which is " + std::to_string(this->size()) + ")";
                throw std::out_of_range(error);
            }
        }

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


        vector<unpack<T>> 
            (std::initializer_list<T> ilist) {
            for (auto& tuple : ilist) {
                push_back(tuple);
            }  
        }

        vector<unpack<T>>& operator=(const vector<unpack<T>>& v) {
            // TODO: is this ok? / reuse of code
            tuple_for_each(v._data, _data, [](auto& _old, auto& _new) {
                _new = _old;
            }); 
            return *this;
        }

        vector<unpack<T>>& operator=(vector<unpack<T>>&& v) {
            tuple_for_each(v._data, _data, [](auto&& _old, auto& _new) {
                _new = std::move(_old); 
            });
            return *this;
        }

        vector<unpack<T>>& operator=(std::initializer_list<T> ilist) {
            for (auto& tuple : ilist) {
                push_back(tuple); 
            }
            return *this;
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

        bool empty() const {
            return size() == 0;
        }

        void push_back(const T& elem) {
            tuple_for_each(elem, _data, [](auto& new_elem, auto& cur_vect) {
                cur_vect.push_back(new_elem);
            });
        }
        
        void reserve(size_t size) {
            tuple_for_each(_data, [size](auto& cur_vect) {
                cur_vect.reserve(size);
            });
        }

        void pop_back() {
            tuple_for_each(_data, [](auto& cur_vect) {
                cur_vect.pop_back();
            });
        }

        void clear() {
            tuple_for_each(_data, [](auto& cur_vect) { cur_vect.clear(); });
        }

        void swap(vector<unpack<T>>& v) {
            tuple_for_each(_data, v._data, [](auto& cur_vect, auto& target_cur_vect) {
                cur_vect.swap(target_cur_vect);
            });
        }

        void resize(size_t size) {
            tuple_for_each(_data, [size](auto& cur_vect) { cur_vect.resize(size); });
        }

        void shrink_to_fit() {
            tuple_for_each(_data, [](auto& cur_vect) { cur_vect.shrink_to_fit(); });
        }

        tuple_refs_type operator[](size_t index) {
            return tuple_r_at_index(_data, index); 
        }

        tuple_const_refs_type operator[](size_t index) const {
            return tuple_r_at_index(_data, index);
        }

        tuple_refs_type at(size_t index) {
            throw_if_out_of_bounds(index);
            return tuple_r_at_index(_data, index);
        }

        tuple_const_refs_type at(size_t index) const {
            throw_if_out_of_bounds(index);
            return tuple_r_at_index(_data, index);
        }

        tuple_refs_type front() {
            return operator[](0);
        }

        tuple_const_refs_type front() const {
            return operator[](0);
        }
        
        tuple_refs_type back() {
            return operator[](size() - 1);
        }

        tuple_const_refs_type back() const {
            return operator[](size() - 1);
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

        class const_iterator {
            public:
                using difference_type = std::ptrdiff_t;
                using value_type = T; 
                using pointer = tuple_const_refs_type*; // TODO
                using reference = tuple_const_refs_type;
                using iterator_category = std::bidirectional_iterator_tag;
            
                template <typename Func>
                const_iterator(const data_type& d, Func&& f)
                    : _data(make_tuple_vec_const_iter(d, std::forward<Func>(f))) 
                {
                }

                const_iterator& operator++() {
                    tuple_for_each(_data, [](auto& iter) { iter++; }); 
                    return *this;     
                }

                const_iterator operator++(int) {
                    vector<unpack<T>>::const_iterator it = *this;
                    tuple_for_each(_data, [](auto& iter) { iter++; }); 
                    return it;    
                } 
                
                const_iterator& operator--() {
                    tuple_for_each(_data, [](auto& iter) { iter--; }); 
                    return *this;     
                }

                const_iterator operator--(int) {
                    vector<unpack<T>>::const_iterator it = *this;
                    tuple_for_each(_data, [](auto& iter) { iter--; }); 
                    return it;    
                }

                tuple_const_refs_type operator*() {
                    return make_tuple_refs(_data); 
                }

                bool operator==(const const_iterator& rhs) {
                    return rhs._data == _data;
                }

                bool operator!=(const const_iterator& rhs) {
                    return rhs._data != _data;       
                }

                template <typename DistType>
                friend DistType distance(const iterator& begin, const iterator& end) {
                   return std::distance(std::get<0>(end), std::get<0>(begin));
                }

                private:
                    tuple_vec_const_iter_type _data; 
        };
    
        
        const_iterator begin() const {
            return const_iterator(_data, [](auto& iter) { return iter.begin(); }); 
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator end() const {
            return const_iterator(_data, [](auto& iter) { return iter.end(); });
        }

        const_iterator cend() const {
            return end();
        }

        template <typename V, typename Alloc>
        friend void swap(vector<unpack<V, Alloc>>& lhs, vector<unpack<V, Alloc>>& rhs) {
            lhs.swap(rhs);
        }
};

}
