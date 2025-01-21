#include "game/resource.hpp"

#include <gtest/gtest.h>

class ResourceTest : public ::testing::Test {
 protected:
  Resource wood = Resource(ResourceType::WOOD, 3);
};

// 初期状態のテスト
TEST_F(ResourceTest, InitialState) {
  EXPECT_EQ(wood.getType(), ResourceType::WOOD);
  EXPECT_EQ(wood.getAmount(), 3);
}

// リソース追加のテスト
TEST_F(ResourceTest, AddResources) {
  wood.add(2);
  EXPECT_EQ(wood.getAmount(), 5);
}

// リソース消費のテスト
TEST_F(ResourceTest, SubtractResources) {
  // 正常系：リソースが十分にある場合
  EXPECT_TRUE(wood.subtract(2));
  EXPECT_EQ(wood.getAmount(), 1);

  // 異常系：リソースが足りない場合
  EXPECT_FALSE(wood.subtract(2));
  EXPECT_EQ(wood.getAmount(), 1);  // 量は変化していないはず
}