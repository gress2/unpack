# unpack

##### What is it?
Commonly, constructs representing compound data types (structs, tuples, arrays, etc.) are stored in multitudes in various standard containers. For example:
```c++
std::vector<std::tuple<int, double, float>> v;
```
When accessing data of this format, cache lines filled with with these tuples, one after another. Depending on how the data is being used by used by the application, it can often be beneficial to structure this type of data differently. Consider the case where all `int` are read, then all `double`, then all `float`. In this scenario, it would be better to store the data in this way:
```c++
std::tuple<std::vector<int>, std::vector<double>, std::vector<float>> t;
```
It would be useful to be able to quickly compare the performance of these two data repsentations without the need to restructure your code to support the differences. This is the goal of `unpack`. `unpack` will be a variadic template class which serves as a marker for its data to be treated differently by standard containers. Consider the following usage:
```c++
std::vector<unpack<std::tuple<int, double, float>>> v;
```
By wrapping the `std::tuple` in an `unpack` we can store the data as if it were a tuple of arrays but program around it as though it was a vector of tuples. 
