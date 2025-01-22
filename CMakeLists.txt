cmake_minimum_required(VERSION 3.10)
set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)

# コンパイラを設定（CMakeプロジェクトの設定前に行う必要があります）
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Agricola)

# C++20を使用する場合
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # GNU拡張を無効化

add_library(agricola_core
    src/game/resource.cpp
    src/game/field.cpp
    src/game/farm.cpp
    src/game/player.cpp
    src/game/action_space.cpp
)

# インクルードディレクトリの設定
target_include_directories(agricola_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# テストの設定
include(GoogleTest)
enable_testing()
# テスト追加用の関数を定義
function(add_agricola_test TEST_NAME)
    add_executable(${TEST_NAME} tests/${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME}
        PRIVATE
            GTest::gtest_main
            agricola_core
    )
    gtest_discover_tests(${TEST_NAME})
endfunction()

function(add_agricola_test_actions TEST_NAME)
    add_executable(${TEST_NAME} tests/actions/${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME}
        PRIVATE
            GTest::gtest_main
            agricola_core
    )
    gtest_discover_tests(${TEST_NAME})
endfunction()

# テストの追加
add_agricola_test(resource_test)
add_agricola_test(field_test)
add_agricola_test(farm_test)
add_agricola_test(player_test)
add_agricola_test(action_space_test)

add_agricola_test_actions(day_labour_test)
add_agricola_test_actions(plow_field_test)
add_agricola_test_actions(gain_grain_test)
add_agricola_test_actions(four_woods_accum_test)
add_agricola_test_actions(three_woods_accum_test)

# add_executable(hello_world src/main.cpp)
