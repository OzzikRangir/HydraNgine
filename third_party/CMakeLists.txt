

# add_library(vkbootstrap OBJECT vkbootstrap/VkBootstrap.cpp)
# target_compile_options(vkbootstrap PUBLIC -fPIC)

add_library(hydra_third_party OBJECT vkbootstrap/VkBootstrap.cpp)
target_compile_options(hydra_third_party PUBLIC -fPIC)