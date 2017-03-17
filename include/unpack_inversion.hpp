#include <tuple>
#include <vector>

template <typename T, typename Indices>
struct inversion_helper;

template <typename T, size_t ... Indices>
struct inversion_helper<T, std::index_sequence<Indices ...>> {
    using type = std::tuple
                    <std::vector<typename std::tuple_element<Indices, T>::type> ...
                 >;
};

template <typename T>
struct unpack_inversion {
    using Indices = std::make_index_sequence<std::tuple_size<T>::value>;
    using type = typename inversion_helper<T, Indices>::type; 
};
