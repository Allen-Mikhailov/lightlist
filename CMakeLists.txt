cmake_minimum_required(VERSION 3.10)

project(lightlist)

# For debugging with gprof
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# for Debugging with prof
set(CMAKE_BUILD_TYPE Debug)

add_executable(lightlist lightlist.c cjson/cJSON.c)

target_compile_options(lightlist PUBLIC -march=native)
target_compile_options(lightlist PUBLIC -ffast-math)

# Link the math library
target_link_libraries(lightlist m)


