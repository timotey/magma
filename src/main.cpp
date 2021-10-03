#include<vector>
#include<vulkan/vulkan.h>
#include"magma/vk.hpp"

int main(int argc, char const ** argv) {
    constexpr auto b = magma::instance_builder()
        .version<magma::make_version<0,1,1,0>>()
        .add_extension<magma::khr::surface>();
    auto inst = b.build();
}
