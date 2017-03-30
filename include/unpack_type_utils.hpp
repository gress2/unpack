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
