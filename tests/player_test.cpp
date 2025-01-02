#include "game/player.hpp"
#include <gtest/gtest.h>

class PlayerTest : public ::testing::Test {
 protected:
  Player player;
};

TEST_F(PlayerTest, InitialState) {
    EXPECT_EQ(player.getFamilyMemberCount(), 2);
    EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(), 3);
    
    // 他のリソースは0で初期化されているはず
    EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(), 0);
    EXPECT_EQ(player.getResource(ResourceType::CLAY).getAmount(), 0);
    // ... 他のリソースも同様
}

TEST_F(PlayerTest, AddResource) {
    player.addResource(Resource(ResourceType::WOOD, 2));
    EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(), 2);
    
    // 同じタイプのリソースを追加
    player.addResource(Resource(ResourceType::WOOD, 3));
    EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(), 5);
}

TEST_F(PlayerTest, UseResource) {
    player.addResource(Resource(ResourceType::WOOD, 5));
    
    EXPECT_TRUE(player.useResource(Resource(ResourceType::WOOD, 3)));
    EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(), 2);
    
    // 不足しているリソースは使用できない
    EXPECT_FALSE(player.useResource(Resource(ResourceType::WOOD, 3)));
    EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(), 2);
} 

TEST_F(PlayerTest, AddFamilyMember) {
    EXPECT_EQ(player.getFamilyMemberCount(), 2);
    
    // 家族を追加できる
    EXPECT_TRUE(player.addFamilyMember());
    EXPECT_EQ(player.getFamilyMemberCount(), 3);
    
    // 最大5人まで追加可能
    EXPECT_TRUE(player.addFamilyMember());
    EXPECT_TRUE(player.addFamilyMember());
    EXPECT_EQ(player.getFamilyMemberCount(), 5);
    
    // これ以上は追加できない
    EXPECT_FALSE(player.addFamilyMember());
    EXPECT_EQ(player.getFamilyMemberCount(), 5);
}

TEST_F(PlayerTest, FeedFamily) {
    // 初期状態: 家族2人、食料3
    EXPECT_EQ(player.getFamilyMemberCount(), 2);
    EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(), 3);
    
    // 家族2人の場合、4食料が必要
    // 3食料しかないので1枚の乞食カードが発生する
    EXPECT_TRUE(player.feedFamily(Resource(ResourceType::FOOD, 3)));
    EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(), 0);
    EXPECT_EQ(player.getBegginCards(), 1);
    
    // 食料以外のリソースでは家族を養えない
    player.addResource(Resource(ResourceType::WOOD, 5));
    EXPECT_FALSE(player.feedFamily(Resource(ResourceType::WOOD, 5)));
    
    // 所持量以上の食料は使用できない
    player.addResource(Resource(ResourceType::FOOD, 2));
    EXPECT_FALSE(player.feedFamily(Resource(ResourceType::FOOD, 3)));
}
