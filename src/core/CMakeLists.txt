# add_library(entity entity/entity.cpp)
# add_library(io io/image_assets.cpp)

add_library(hydra_base STATIC context.cpp base.cpp)
target_compile_options(hydra_base PRIVATE -fPIC)