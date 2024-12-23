cmake_minimum_required(VERSION 3.10)

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
    src/game/Resource.cpp
    src/game/Field.cpp
    # src/game/Farm.cpp
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
enable_testing()
add_executable(resource_test tests/ResourceTest.cpp)
target_link_libraries(resource_test
    PRIVATE
        GTest::gtest_main
        agricola_core  # あなたのゲームエンジンライブラリ
)
add_executable(field_test tests/FieldTest.cpp)
target_link_libraries(field_test
    PRIVATE
        GTest::gtest_main
        agricola_core
)

include(GoogleTest)
gtest_discover_tests(resource_test)
gtest_discover_tests(field_test)

# add_executable(hello_world src/main.cpp)