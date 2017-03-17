#include <vector>
#include <iostream>
#include "is_stl_container.hpp"
#include "unpack_inversion.hpp"

namespace std
{

template <typename T>
void f(T t) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template <typename T>
class vector<unpack<T>> {
    private:
        using data_type = typename unpack_inversion<T>::type;
        data_type data_;
    public:
        template <typename U = T>
        vector<unpack<T>>(typename std::enable_if<is_stl_container<U>::value>::type * = NULL) {
            f(data_);
        }
};

}
