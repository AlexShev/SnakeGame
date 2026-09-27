cmake_minimum_required(VERSION 3.22)
project(SnakeGame LANGUAGES CXX)

# Point this at the directory containing share/cmake/Torch/TorchConfig.cmake.
# An explicit -DTorch_DIR=... or -DCMAKE_PREFIX_PATH=... also works.
set(LIBTORCH_ROOT "" CACHE PATH "Path to the unpacked LibTorch directory")
if(LIBTORCH_ROOT)
    list(PREPEND CMAKE_PREFIX_PATH "${LIBTORCH_ROOT}")
elseif(DEFINED ENV{LIBTORCH_ROOT} AND NOT "$ENV{LIBTORCH_ROOT}" STREQUAL "")
    list(PREPEND CMAKE_PREFIX_PATH "$ENV{LIBTORCH_ROOT}")
endif()

find_package(Torch CONFIG QUIET)
if(NOT Torch_FOUND)
    message(FATAL_ERROR
        "LibTorch was not found. Download and unpack the Windows x64 Release archive, "
        "then configure with -DLIBTORCH_ROOT=C:/path/to/libtorch (the directory "
        "containing share/cmake/Torch/TorchConfig.cmake). "
        "Alternatively set Torch_DIR to the directory containing TorchConfig.cmake.")
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

set(SNAKE_CORE
    pyton/Field.cpp
    pyton/Snake.cpp
    pyton/HardArtificialGamer.cpp
    pyton/NeuralArtificialGamer.cpp)

if(WIN32)
    add_executable(SnakeGame
        pyton/pyton.cpp
        pyton/Game.cpp
        pyton/ConsoleController.cpp
        pyton/Gamer.cpp
        pyton/StupedArtificialGamer.cpp
        pyton/AverageArtificialGamer.cpp
        ${SNAKE_CORE})
    target_compile_definitions(SnakeGame PRIVATE NOMINMAX)
    target_include_directories(SnakeGame PRIVATE pyton)
    target_link_libraries(SnakeGame PRIVATE "${TORCH_LIBRARIES}")
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET SnakeGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SnakeGame>/models"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/models/neural_policy.pt"
            "$<TARGET_FILE_DIR:SnakeGame>/models/neural_policy.pt"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS} "$<TARGET_FILE_DIR:SnakeGame>"
        VERBATIM)
endif()

add_executable(NeuralBenchmark training/benchmark.cpp ${SNAKE_CORE})
target_include_directories(NeuralBenchmark PRIVATE . pyton)
target_link_libraries(NeuralBenchmark PRIVATE "${TORCH_LIBRARIES}")
add_custom_command(TARGET NeuralBenchmark POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:NeuralBenchmark>/models"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/models/neural_policy.pt"
        "$<TARGET_FILE_DIR:NeuralBenchmark>/models/neural_policy.pt"
    VERBATIM)

if(MSVC)
    add_custom_command(TARGET NeuralBenchmark POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS} "$<TARGET_FILE_DIR:NeuralBenchmark>"
        VERBATIM)
endif()

add_executable(NeuralCollect training/collect.cpp ${SNAKE_CORE})
target_include_directories(NeuralCollect PRIVATE . pyton)
target_link_libraries(NeuralCollect PRIVATE "${TORCH_LIBRARIES}")
if(MSVC)
    add_custom_command(TARGET NeuralCollect POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS} "$<TARGET_FILE_DIR:NeuralCollect>"
        VERBATIM)
endif()
