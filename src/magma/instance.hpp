#ifndef MAGMA_INSTANCE_HPP__
#define MAGMA_INSTANCE_HPP__
#include<utility>
#include<tuple>
#include<string_view>
#include<vulkan/vulkan.h>
#include"detail/util.hpp"

namespace magma {

template<class U>
concept Extension = requires {
    {U::name} -> std::convertible_to<std::string_view>;
} && std::default_initializable<U>;
template<class U>
concept Next = requires (U x, void* p) {
    {x | p} -> std::same_as<void*>;
};

struct version {
    std::uint32_t x;
    constexpr operator std::uint32_t() const {
        return x;
    }
};

template<
    std::uint8_t variant,
    std::uint8_t major,
    std::uint8_t minor,
    std::uint8_t patch>
constexpr version make_version = version{
    std::uint32_t(variant) << 29 |
    std::uint32_t( major ) << 22 |
    std::uint32_t( minor ) << 12 |
    std::uint32_t( patch ) << 00 
};

template<class Extensions, version Version, auto UUID = []{}>
class instance; 

template<class Extensions = std::tuple<>,
    class Nexts = std::tuple<>,
    class Layers = std::tuple<>,
    version Version = version{0}
    >
struct instance_builder{
    private:
template<class E,
    class N,
    class L,
    version V
    >
    friend struct instance_builder;
    constexpr instance_builder(Nexts&& n, VkApplicationInfo&& i):
        nexts(std::move(n)), appinfo(std::move(i)){}
    template<class E, class L, version V>
    constexpr instance_builder(instance_builder<E, Nexts, L, V>&& i):
        nexts(i.nexts), appinfo(i.appinfo){}
    template<class E, class L, version V>
    constexpr instance_builder(instance_builder<E, Nexts, L, V> const& i):
        nexts(i.nexts), appinfo(i.appinfo){}

    Nexts nexts;
    VkApplicationInfo appinfo;
    public:
    constexpr instance_builder():nexts(), appinfo(VkApplicationInfo{
            .sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext=nullptr,
            .pApplicationName = nullptr,
            .applicationVersion=0,
            .pEngineName = nullptr,
            .engineVersion = 0,
            .apiVersion=Version,
            }){
    }
    template<Extension Ext>
    instance_builder<
        _detail::tuple_cat_t<std::tuple<Ext>, Extensions>,
        Nexts,
        Layers,
        Version>
    constexpr add_extension() requires Extension<Ext> {
        return instance_builder{std::move(nexts),std::move(appinfo)};
    }

    template<class Next>
    instance_builder<
        Extensions,
        _detail::tuple_cat_t<std::tuple<Next>, Nexts>,
        Layers,
        Version>
    constexpr add_next(Next&& s) {
    return instance_builder<
        Extensions,
        _detail::tuple_cat_t<std::tuple<Next>, Nexts>,
        Layers,
        Version>{std::tuple_cat(std::tie(s), nexts), appinfo};
    }

    constexpr instance_builder add_layer(){
        return *this;
    }

    constexpr instance_builder title(std::string_view);

    template<version Ver>
    constexpr instance_builder<Extensions, Nexts, Layers, Ver> version(){
        return *this;
    }

    instance<Extensions, Version> build() const requires
        (Version >= std::int32_t(make_version<0,1,0,0>) || Version == 0)
        {
        auto nexts2 = nexts;
        void* pNext = std::apply(
            [](auto&&... args){
                return (args | ... | nullptr);
            }, nexts2); //link all pNexts together

        std::array<char const *, std::tuple_size_v<Extensions>> exts;
        std::apply([&exts](auto&&... xs) {
                ([i=0u, &exts](auto&& x) mutable{
                 using X = std::remove_cvref_t<decltype(x)>;
                 exts[i++] = X::name.data();
                }(xs), ...);
            }, Extensions{});//add all extensions
        std::array<char const *, std::tuple_size_v<Extensions>> layers;
        std::apply([&exts](auto&&... xs) {
                ([i=0u, &exts](auto&& x) mutable{
                 using X = std::remove_cvref_t<decltype(x)>;
                 exts[i++] = X::name.data();
                }(xs), ...);
                }, Layers{});//add all extensions
        VkInstanceCreateInfo info {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = pNext,
            .flags = 0,
            .pApplicationInfo = &appinfo,
            .enabledLayerCount = layers.size(),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = exts.size(),
            .ppEnabledExtensionNames = exts.data()
        };
        VkInstance d;
        vkCreateInstance(&info, nullptr, &d);
        return d;
    }
};

template<class Extensions, version Version, auto UUID>
class instance {
    template<class Exts, class Layers, class Nexts, version xVersion>
    friend struct instance_builder;
    instance(VkInstance t): inst(t){}
    VkInstance inst;
};


}
#endif // MAGMA_INSTANCE_HPP__
