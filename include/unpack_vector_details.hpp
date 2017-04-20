#include <tuple>
#include <vector>

template <typename T, typename Indices>
struct inversion_helper;

template <typename T, size_t ... Indices>
struct inversion_helper<T, std::index_sequence<Indices ...>> {
    using type = std::tuple<
                    std::vector<typename std::tuple_element<Indices, T>::type> ...
                 >;
};

template <typename T>
struct unpack_inversion {
    using Indices = std::make_index_sequence<std::tuple_size<T>::value>;
    using type = typename inversion_helper<T, Indices>::type; 
};

template <typename T, typename Indices>
struct tuple_refs_type_helper;

template <typename T, size_t ... Indices>
struct tuple_refs_type_helper<T, std::index_sequence<Indices ...>> {
    using type = std::tuple<
                    typename std::tuple_element<Indices, T>::type& ...
                 >;
};

template <typename T>
struct unpack_tuple_refs_type {
    using Indices = std::make_index_sequence<std::tuple_size<T>::value>;
    using type = typename tuple_refs_type_helper<T, Indices>::type;
};

template <typename T, typename Indices>
struct tuple_const_refs_type_helper;

template <typename T, size_t ... Indices>
struct tuple_const_refs_type_helper<T, std::index_sequence<Indices ...>> {
    using type = std::tuple<
                    const typename std::tuple_element<Indices, T>::type& ...
                 >;
};

template <typename T>
struct unpack_tuple_const_refs_type {
    using Indices = std::make_index_sequence<std::tuple_size<T>::value>;
    using type = typename tuple_const_refs_type_helper<T, Indices>::type;
};

template <typename T, typename Indices>
struct tuple_vec_iter_type_helper;

template <typename T, size_t ... Indices>
struct tuple_vec_iter_type_helper<T, std::index_sequence<Indices ...>> {
    using type = std::tuple<
                    typename std::vector<typename std::tuple_element<Indices, T>::type>::iterator ... 
                 >;
};

template <typename T>
struct unpack_tuple_vec_iter_type {
    using Indicies = std::make_index_sequence<std::tuple_size<T>::value>;
    using type = typename tuple_vec_iter_type_helper<T, Indicies>::type;
};

template <typename T, typename Indices>
struct tuple_vec_const_iter_type_helper;

template <typename T, size_t ... Indices>
struct tuple_vec_const_iter_type_helper<T, std::index_sequence<Indices ...>> {
    using type = std::tuple<
                    typename std::vector<typename std::tuple_element<Indices, T>::type>::const_iterator ... 
                 >;
};

template <typename T>
struct unpack_tuple_vec_const_iter_type {
    using Indicies = std::make_index_sequence<std::tuple_size<T>::value>;
    using type = typename tuple_vec_const_iter_type_helper<T, Indicies>::type;
};

template <typename Tuple, typename Func, std::size_t ... Indices>
void tuple_for_each_helper(Tuple&& tuple, Func&& f, std::index_sequence<Indices ...>) {
    // black magic to ensure function calls occur in order
    using swallow = int[];
    (void)swallow{1, (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...};
}

template <typename Tuple, typename Func>
void tuple_for_each(Tuple&& tuple, Func&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    tuple_for_each_helper(std::forward<Tuple>(tuple), std::forward<Func>(f),
                    std::make_index_sequence<N>{});
}

template <typename Tuple1, typename Tuple2, typename Func, std::size_t ... Indices>
void tuple_for_each_helper(Tuple1&& tuple1, Tuple2&& tuple2, Func&& f, std::index_sequence<Indices ...>) {
    using swallow = int[];
    (void)swallow{1, (f(std::get<Indices>(std::forward<Tuple1>(tuple1)),
                        std::get<Indices>(std::forward<Tuple2>(tuple2))), void(), int{})...};
}

template <typename Tuple1, typename Tuple2, typename Func>
void tuple_for_each(Tuple1&& tuple1, Tuple2&& tuple2, Func&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple1>>::value;
    tuple_for_each_helper(std::forward<Tuple1>(tuple1), std::forward<Tuple2>(tuple2),
                          std::forward<Func>(f), std::make_index_sequence<N>{});
}

// TODO: Generalize this function to work for any amount of tuples without overloading like this
template <typename Tuple1, typename Tuple2, typename Tuple3, typename Func, std::size_t ... Indices>
void tuple_for_each_helper(Tuple1&& tuple1, Tuple2&& tuple2, Tuple3&& tuple3, Func&& f, std::index_sequence<Indices ...>) {
    using swallow = int[];
    (void)swallow{1, (f(std::get<Indices>(std::forward<Tuple1>(tuple1)),
                        std::get<Indices>(std::forward<Tuple2>(tuple2)),
                        std::get<Indices>(std::forward<Tuple3>(tuple3))
                        ), void(), int{})...};
}

template <typename Tuple1, typename Tuple2, typename Tuple3, typename Func>
void tuple_for_each(Tuple1&& tuple1, Tuple2&& tuple2, Tuple3&& tuple3, Func&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple1>>::value;
    tuple_for_each_helper(std::forward<Tuple1>(tuple1), std::forward<Tuple2>(tuple2), std::forward<Tuple3>(tuple3),
                          std::forward<Func>(f), std::make_index_sequence<N>{});
}

/*
template <typename Func, std::size_t ... Indices, typename ... Tuples>
void tuple_for_each_helper(Func&& f, std::index_sequence<Indices ...>, Tuples&& ... tuples) {
    using swallow = int[];
    (void)swallow{1, (f(std::get<Indices>(std::forward<Tuples>(tuples))), void(), int{})... };
}

template <typename Func, typename ... Tuples>
void tuple_for_each(Func&& f, Tuples&& ... tuples) {
    using first_tuple_type = typename std::tuple_element<0, std::tuple<Tuples ... >>::type;
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<first_tuple_type>>::value;
    tuple_for_each_helper(std::forward<Func>(f), std::make_index_sequence<N>{}, std::forward<Tuples>(tuples) ...);
}
*/
template <typename Tuple, std::size_t ... Indices>
auto tuple_r_at_index_helper(Tuple&& tuple, std::size_t index, std::index_sequence<Indices ...>) {
    return std::tie(std::get<Indices>(std::forward<Tuple>(tuple))[index] ... );
}

template <typename Tuple> 
auto tuple_r_at_index(Tuple&& tuple, std::size_t index) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    return tuple_r_at_index_helper(std::forward<Tuple>(tuple), 
                                   index, std::make_index_sequence<N>{}); 
} 

template <typename DataType, typename Func, std::size_t ... Indices>
auto make_tuple_vec_iter_helper(DataType&& data, Func&& f, std::index_sequence<Indices ...>) {
    return std::make_tuple(f(std::get<Indices>(std::forward<DataType>(data))) ... );
} 

template <typename DataType, typename Func>
auto make_tuple_vec_iter(DataType&& data, Func&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<DataType>>::value;
    return make_tuple_vec_iter_helper(std::forward<DataType>(data), 
            std::forward<Func>(f), std::make_index_sequence<N>{});
} 

template <typename DataType, typename Func, std::size_t ... Indices>
auto make_tuple_vec_const_iter_helper(DataType&& data, Func&& f, std::index_sequence<Indices ...>) {
    return std::make_tuple(f(std::get<Indices>(std::forward<DataType>(data))) ... );
} 

template <typename DataType, typename Func>
auto make_tuple_vec_const_iter(DataType&& data, Func&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<DataType>>::value;
    return make_tuple_vec_const_iter_helper(std::forward<DataType>(data), 
            std::forward<Func>(f), std::make_index_sequence<N>{});
} 

template <typename DataType, std::size_t ... Indices>
auto make_tuple_refs_helper(DataType&& data, std::index_sequence<Indices ... >) {
    return std::tie(*std::get<Indices>(std::forward<DataType>(data)) ... );
}

template <typename DataType>
auto make_tuple_refs(DataType&& data) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<DataType>>::value;
    return make_tuple_refs_helper(std::forward<DataType>(data), std::make_index_sequence<N>{});
}
