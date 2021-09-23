#ifndef MAGMA_INSTANCE_KHR_HPP__
#define MAGMA_INSTANCE_KHR_HPP__
#include"../instance.hpp"

namespace magma::khr {

template<class Base>
struct surface;

template<template<class> class... Ts>
struct surface<instance<Ts...>>{
    static constexpr std::string_view name = "VK_KHR_SURFACE";
};

}
#endif //MAGMA_INSTANCE_KHR_HPP__
