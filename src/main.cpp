#include<vector>
#include<vulkan/vulkan.h>
#include"magma/vk.hpp"

int main(int argc, char const ** argv) {
    VkApplicationInfo app {
        .pApplicationName = "Hello",
        .applicationVersion = 1,
        .pEngineName = "Einginigger",
        .engineVersion = 1,
        .apiVersion = VK_API_VERSION_1_1
    };
    magma::instance inst = magma::instance_builder()
        .add_extension<magma::khr::surface>()
        .build();
}
