cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
cmake_policy(VERSION 3.7.0)

project(HydraNgine VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CPP_COMPILER /usr/bin/g++)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-dynamic")
find_package(Vulkan REQUIRED)
# include(external/tbb.cmake)
# add_subdirectory(external/vku)
# MACRO(HEADER_DIRECTORIES return_list)
#     FILE(GLOB_RECURSE new_list include/*.hpp)
#     SET(dir_list "")
#     FOREACH(file_path ${new_list})
#         GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
#         SET(dir_list ${dir_list} ${dir_path})
#     ENDFOREACH()
#     LIST(REMOVE_DUPLICATES dir_list)
#     SET(${return_list} ${dir_list})
# ENDMACRO()


# HEADER_DIRECTORIES(header_dir_list)
# list(LENGTH header_dir_list header_dir_list_count)

# include_directories(
#     ${header_dir_list} # Recursive
# )
# add_subdirectory(external/vku)
include_directories(include)
include_directories(third_party)
add_subdirectory(third_party)
# set(_GLFW_X11 1)
# include_directories(external/vku/include)
# add_library(hydra  src/dummy.c)
add_subdirectory(src/core)
add_subdirectory(src/drivers)


add_library(hydra SHARED $<TARGET_OBJECTS:hydra_third_party> $<TARGET_OBJECTS:hydra_base>)
target_compile_options(hydra PRIVATE -fPIC)
# set_target_properties(hydra PROPERTIES LINKER_LANGUAGE C)
# target_link_libraries(hydra ${CMAKE_DL_LIBS} glfw Vulkan::Vulkan X11 Xrandr Xinerama Xcursor)

# target_include_directories(vulkan PUBLIC external/vku/include)
# target_link_libraries(hydra GL)
# target_link_libraries(hydra GLU)

# target_link_libraries(hydra PRIVATE  )
# target_link_libraries(hydra PUBLIC ${CMAKE_DL_LIBS} glfw Vulkan::Vulkan X11 Xrandr Xinerama Xcursor)

# target_precompile_headers(hydra
#   INTERFACE
#    include/core.hpp
# )

add_executable(HydraNgine-test src/HydraNgine.cpp)
# add_dependencies(HydraNgine-test hydra)

# target_include_directories(HydraNgine-test PRIVATE third_party/include)
# target_link_libraries(hydra ${CMAKE_DL_LIBS} )
target_link_libraries (HydraNgine-test hydra hydra_glfw hydra_vulkan glfw Vulkan::Vulkan)
# target_link_libraries (HydraNgine-test ${CMAKE_DL_LIBS} glfw Vulkan::Vulkan X11 Xrandr Xinerama Xcursor)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()