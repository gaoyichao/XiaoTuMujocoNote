cmake_minimum_required(VERSION 3.17)
project(learning_mujoco)
set(PACKAGE_VERSION "0.0.1")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_compile_options(-std=c++17)
set(CMAKE_BUILD_TYPE DEBUG)

message("=======================================")

message("${PROJECT_NAME}")
message("${PROJECT_SOURCE_DIR}")
message("${PROJECT_BINARY_DIR}")
message("${CMAKE_INSTALL_PREFIX}")

set(MuJoCo_Include_Dir /home/gyc/Project/mujoco-2.3.7-linux-x86_64/mujoco-2.3.7/include)
set(MuJoCo_Library_Dir /home/gyc/Project/mujoco-2.3.7-linux-x86_64/mujoco-2.3.7/lib)
set(MuJoCo_Libraries mujoco)


include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${MuJoCo_Include_Dir}
)

link_directories(
    ${MuJoCo_Library_Dir}
)

add_executable(test ./test.cpp)
target_link_libraries(test ${MuJoCo_Libraries})


add_executable(test_with_gui ./test_with_gui.cpp)
target_link_libraries(test_with_gui
    ${MuJoCo_Libraries}
    glfw
)


