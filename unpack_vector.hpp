#include <vector>
#include <iostream>

namespace std
{

template <typename T>
class vector<unpack<T>> {
    private:
    public:
        vector<unpack<T>>() {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }
};

}
