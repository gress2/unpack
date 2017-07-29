template <typename, typename>
struct append_to_type {};

template <typename T, typename... Ts>
struct append_to_type<T, std::tuple<Ts...>> {
  using type = std::tuple<Ts..., T>;
};

template <typename T, std::size_t N>
struct repeat_type_tuple {
  using type = typename append_to_type<T, typename repeat_type_tuple<T, N - 1>::type>::type; 
};

template <typename T>
struct repeat_type_tuple<T, 0> {
  using type = std::tuple<>;
};
