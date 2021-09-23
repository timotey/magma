#ifndef MAGMA_INSTANCE_HPP__
#define MAGMA_INSTANCE_HPP__
#include<utility>
#include<string_view>

namespace magma {

template<template<class Instance> class... Extension>
class instance; 

template<template<class> class U, template<template<class>class...> class T>
concept Extension = requires {
    {U<T<U>>::name} -> std::convertible_to<std::string_view>;
};

template<class Extensions = std::tuple<>,
    class Nexts = std::tuple<>>
struct instance_builder{
    template<template<class> class Ext>
    instance_builder add_extension() requires Extension<Ext , instance>;
    instance<> build();
};

template<template<class Instance> class... Extension>
class instance : public Extension<instance<Extension...>>... {
    template<class State>
    friend class instance_builder;
    instance();
};


}
#endif // MAGMA_INSTANCE_HPP__
