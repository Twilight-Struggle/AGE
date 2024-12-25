#include <gtest/gtest.h>
#include "game/resource.hpp"

class ResourceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 各テストケース前の共通セットアップ
        wood = new Resource(ResourceType::WOOD, 3);
    }

    void TearDown() override {
        delete wood;
    }

    Resource* wood;
};

// 初期状態のテスト
TEST_F(ResourceTest, InitialState) {
    EXPECT_EQ(wood->getType(), ResourceType::WOOD);
    EXPECT_EQ(wood->getAmount(), 3);
}

// リソース追加のテスト
TEST_F(ResourceTest, AddResources) {
    wood->add(2);
    EXPECT_EQ(wood->getAmount(), 5);
}

// リソース消費のテスト
TEST_F(ResourceTest, SubtractResources) {
    // 正常系：リソースが十分にある場合
    EXPECT_TRUE(wood->subtract(2));
    EXPECT_EQ(wood->getAmount(), 1);

    // 異常系：リソースが足りない場合
    EXPECT_FALSE(wood->subtract(2));
    EXPECT_EQ(wood->getAmount(), 1);  // 量は変化していないはず
} 