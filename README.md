# unpack

### What is it?
Commonly, constructs representing compound data types (structs, tuples, arrays, etc.) are stored in multitudes in various standard containers. For example:
```c++
std::vector<std::tuple<int, double, float>> v;
```
When accessing data of this format, cache lines filled with with these tuples, one after another. Depending on how the data is being used by used by the application, it can often be beneficial to structure this type of data differently. Consider the case where all `int` are read, then all `double`, then all `float`. In this scenario, it would be better to store the data in this way:
```c++
std::tuple<std::vector<int>, std::vector<double>, std::vector<float>> t;
```
It would be useful to be able to quickly compare the performance of these two data repsentations without the need to restructure your code to support the differences. This is the goal of `unpack`. `unpack` will be a template class which serves as a marker for its data to be treated differently by standard containers. Consider the following usage:
```c++
std::vector<unpack<std::tuple<int, double, float>>> v;
```
By wrapping the `std::tuple` in an `unpack` we can store the data as if it were a tuple of vectors but program around it as though it was a vector of tuples. `unpack` depends upon a template specialization of `std::vector`.

### Usage
##### Supported types
1. POD
2. Standard containers

##### Instantiation
Here are a few examples of how you might instantiate a `std::vector<unpack<T>>`:
```c++
struct A {
    int x;
    double y;
};
std::vector<unpack<A>> v1;
std::vector<unpack<std::array<int, 2>>> v2;
std::vector<unpack<std::tuple<char, float>>> v3;
```
##### Modifying
You can modify a `std::vector<unpack<T>>` in the same way in which you would modify `std::vector<T>`.

```c++
std::vector<unpack<std::array<int, 2>>> v;
v.push_back(std::array<int, 2>(3, 4));
v.erase(v.begin());
```
##### Accessing elements
Using the `[]` operator and other element access functions will return an element of type `T` with "data members" as references for `std::vector<unpack<T>>`. For example, given `std::vector<unpack<std::pair<int, double>>>`, `[]` returns an element of type `std::pair<int&, double&>`. 
```c++
std::vector<unpack<std::pair<int, double>>> v;
v.push_back(std::pair<int, double>(1, 1.0f));
double tmp = v[0].second;
```
##### Iterators
Unsurprisingly, a `std::vector<unpack<T>>::iterator` will "point" to an element of type `T` (tentative).
```c++
std::vector<unpack<std::array<int, 2>>> v;
...
std::vector<unpack<std::array<int, 2>>>::iterator iter;
for (iter = v.begin(); iter != v.end(); ++iter) {
    std::cout << (*iter)[0] << "," << (*iter)[1] << std::endl; 
}
```
