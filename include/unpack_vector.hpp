#ifndef UNPACK_VECTOR
#define UNPACK_VECTOR

#include <vector> 
#include <iostream>
#include "unpack_iterator.hpp"
#include "unpack_details.hpp"

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
        using iterator = unpack_iterator<tuple_vec_iter_type>;
        using const_iterator = unpack_const_iterator<tuple_vec_const_iter_type>;

    public:
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;

        vector<unpack<T>>() {}

        vector<unpack<T>>(const vector<unpack<T>>& v) {
            tuple_for_each([](auto& _old, auto& _new) {
                _new = _old;
            }, v._data, _data);
        }

        vector<unpack<T>>(vector<unpack<T>>&& v) {
            tuple_for_each([](auto&& _old, auto& _new) {
                _new = std::move(_old); 
            }, v._data, _data);
        }

        vector<unpack<T>> 
            (std::initializer_list<T> ilist) {
            for (auto& tuple : ilist) {
                push_back(tuple);
            }  
        }

        vector<unpack<T>>& operator=(const vector<unpack<T>>& v) {
            // TODO: is this ok? / reuse of code
            tuple_for_each([](auto& _old, auto& _new) {
                _new = _old;
            }, v._data, _data); 
            return *this;
        }

        vector<unpack<T>>& operator=(vector<unpack<T>>&& v) {
            tuple_for_each([](auto&& _old, auto& _new) {
                _new = std::move(_old); 
            }, v._data, _data);
            return *this;
        }

        vector<unpack<T>>& operator=(std::initializer_list<T> ilist) {
            clear();
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
            tuple_for_each([](auto& new_elem, auto& cur_vect) {
                cur_vect.push_back(new_elem);
            }, elem, _data);
        }
        
        void reserve(size_t size) {
            tuple_for_each([size](auto& cur_vect) {
                cur_vect.reserve(size);
            }, _data);
        }

        void pop_back() {
            tuple_for_each([](auto& cur_vect) {
                cur_vect.pop_back();
            }, _data);
        }

        void clear() {
            tuple_for_each([](auto& cur_vect) { cur_vect.clear(); }, _data);
        }

        iterator insert(const_iterator pos, const T& value) {
            tuple_for_each([](auto& data_vect, auto& vect_iter, auto& tuple_element) {
                vect_iter = data_vect.insert(vect_iter, tuple_element);
            }, _data, *(pos.data()), value);

            iterator it = begin();
            std::advance(it, std::distance(cbegin(), pos)); 
            return it;
        }
       
        iterator insert(const_iterator pos, T&& value ) {
            tuple_for_each([](auto& cur_vect, auto& cur_iter, auto&& tuple_element) {
                cur_iter = cur_vect.insert(cur_iter, std::forward<decltype(tuple_element)>(tuple_element));
            }, _data, *(pos.data()), std::forward<T>(value));
            iterator it = begin();
            std::advance(it, std::distance(cbegin(), pos));
            return it;
        }

        iterator insert(const_iterator pos, size_type count, const T& value ) {
            tuple_for_each([count](auto& cur_vect, auto& cur_iter, auto& tuple_element) {
                cur_iter = cur_vect.insert(cur_iter, count, tuple_element);
            }, _data, *(pos.data()), value);
            iterator it = begin();
            std::advance(it, std::distance(cbegin(), pos));
            return it;
        }

        template <typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            tuple_for_each([](auto& cur_vect, auto& cur_iter, auto& first_iter, auto& last_iter) {
                cur_iter = cur_vect.insert(cur_iter, first_iter, last_iter);
            }, _data, *(pos.data()), *(first.data()), *(last.data())); 
            iterator it = begin();
            std::advance(it, std::distance(cbegin(), pos));
            return it;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
            auto end = ilist.end(); 
            iterator it = begin();
            for (auto cur = ilist.end(); cur != ilist.begin(); ) {
                cur--;
                it = insert(pos, *cur);
            }
            
            return it;
        }

        void swap(vector<unpack<T>>& v) {
            tuple_for_each([](auto& cur_vect, auto& target_cur_vect) {
                cur_vect.swap(target_cur_vect);
            }, _data, v._data);
        }

        void resize(size_t size) {
            tuple_for_each([size](auto& cur_vect) { cur_vect.resize(size); }, _data);
        }

        void shrink_to_fit() {
            tuple_for_each([](auto& cur_vect) { cur_vect.shrink_to_fit(); }, _data);
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

        iterator begin() {
            return iterator(make_tuple_vec_iter(_data, [](auto& iter) { return iter.begin(); }));
        }

        iterator end() {
            return iterator(make_tuple_vec_iter(_data, [](auto& iter) { return iter.end(); }));
        }
        
        const_iterator begin() const {
            return const_iterator(make_tuple_vec_const_iter(_data, [](auto& iter) { return iter.begin(); }));
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator end() const {
            return const_iterator(make_tuple_vec_const_iter(_data, [](auto& iter) { return iter.end(); }));
        }

        const_iterator cend() const {
            return end();
        }

        ~vector() {}

        friend void swap(vector<unpack<T>>& lhs, vector<unpack<T>>& rhs) {
            lhs.swap(rhs);
        }
};

}

#endif
