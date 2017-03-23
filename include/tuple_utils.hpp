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
