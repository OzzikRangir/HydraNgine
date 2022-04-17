add_library(hydra_vulkan SHARED vulkan/memory.cpp 
                                vulkan/descriptors.cpp 
                                vulkan/window.cpp 
                                vulkan/commands.cpp 
                                vulkan.cpp)
add_library(hydra_glfw SHARED glfw.cpp)
target_link_libraries(hydra_vulkan PUBLIC Vulkan::Vulkan)
target_link_libraries(hydra_glfw PUBLIC glfw)
