#include "game/farm.hpp"

#include <gtest/gtest.h>

class FarmTest : public ::testing::Test {
 protected:
  Farm farm;
};
/*
TEST_F(FarmTest, InitialState) {
    // 初期状態で2部屋あることを確認
    EXPECT_EQ(farm.getNumRooms(), 2);
    EXPECT_EQ(farm.getNumStables(), 0);
}
*/

TEST_F(FarmTest, PlowField) {
  // 木の家の上には耕せない
  EXPECT_FALSE(farm.plowField(Position(1, 0)));
  // 最初の畑は任意の場所に作れる
  auto pos1 = Position(1, 1);
  EXPECT_TRUE(farm.plowField(pos1));
  EXPECT_EQ(farm.getField(pos1).getType(), FieldType::FIELD);

  // 2つ目の畑は最初の畑に隣接している必要がある
  auto pos2 = Position(1, 2);
  EXPECT_TRUE(farm.plowField(pos2));
  EXPECT_EQ(farm.getField(pos2).getType(), FieldType::FIELD);
  EXPECT_FALSE(farm.plowField(Position(0, 0)));
}

TEST_F(FarmTest, BuildFence) {
  std::vector<FencePosition> fences = {
      FencePosition::create(Position(0, 0), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(0, 0), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(1, 0), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(2, 0), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(2, 1), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 2), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(0, 2), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(0, 1), FencePosition::Edge::TOP).value(),
  };
  // 空でない場所に柵はおけない
  EXPECT_FALSE(farm.buildFence(fences));

  std::vector<FencePosition> fences2 = {
      FencePosition::create(Position(0, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(0, 3), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(1, 3), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(2, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(2, 4), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 4), FencePosition::Edge::RIGHT).value(),
      FencePosition::create(Position(0, 4), FencePosition::Edge::RIGHT).value(),
      FencePosition::create(Position(0, 4), FencePosition::Edge::TOP).value(),
  };
  // 有効な柵の配置
  EXPECT_TRUE(farm.buildFence(fences2));
  // 柵で囲まれた領域がPASTUREになっていることを確認
  EXPECT_EQ(farm.getField(Position(0, 3)).getType(), FieldType::PASTURE);
  EXPECT_EQ(farm.getField(Position(1, 3)).getType(), FieldType::PASTURE);
  EXPECT_EQ(farm.getField(Position(0, 4)).getType(), FieldType::PASTURE);
  EXPECT_EQ(farm.getField(Position(1, 4)).getType(), FieldType::PASTURE);

  std::vector<FencePosition> fences3 = {
      FencePosition::create(Position(1, 3), FencePosition::Edge::TOP).value(),
  };
  // 中途半端な柵は建設できない
  EXPECT_FALSE(farm.buildFence(fences3));

  std::vector<FencePosition> fences4 = {
      FencePosition::create(Position(1, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 4), FencePosition::Edge::TOP).value(),
  };
  // 牧場をきちんと分けられる柵は建設可能
  EXPECT_TRUE(farm.buildFence(fences4));

  std::vector<FencePosition> fences5 = {
      FencePosition::create(Position(2, 2), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(2, 2), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(2, 2), FencePosition::Edge::BOTTOM)
          .value(),
      FencePosition::create(Position(2, 3), FencePosition::Edge::LEFT).value(),
  };
  // 孤立した柵は建設できない
  EXPECT_FALSE(farm.buildFence(fences5));

  std::vector<FencePosition> fences6 = {
      FencePosition::create(Position(0, 2), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 2), FencePosition::Edge::TOP).value(),
  };
  // 開放部分がある柵は建設できない
  EXPECT_FALSE(farm.buildFence(fences6));

  std::vector<FencePosition> fences7 = {
      FencePosition::create(Position(0, 2), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(0, 2), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(1, 2), FencePosition::Edge::TOP).value(),
  };
  // 外側に新たに柵を建設可能
  EXPECT_TRUE(farm.buildFence(fences7));
  // 柵で囲まれた領域がPASTUREになっていることを確認
  EXPECT_EQ(farm.getField(Position(0, 2)).getType(), FieldType::PASTURE);

  std::vector<FencePosition> fences8 = {
      FencePosition::create(Position(0, 1), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(0, 1), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(1, 1), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(2, 1), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(2, 2), FencePosition::Edge::TOP).value(),
  };
  // 15本以上柵は建設できない
  EXPECT_FALSE(farm.buildFence(fences8));
}

TEST_F(FarmTest, FenceAndStable) {
  std::vector<FencePosition> fences = {
      FencePosition::create(Position(0, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(0, 3), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(1, 3), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(2, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(2, 4), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 4), FencePosition::Edge::RIGHT).value(),
      FencePosition::create(Position(0, 4), FencePosition::Edge::RIGHT).value(),
      FencePosition::create(Position(0, 4), FencePosition::Edge::TOP).value(),
  };
  // 有効な柵の配置
  EXPECT_TRUE(farm.buildFence(fences));

  // 建物などの上には建設できない
  EXPECT_FALSE(farm.buildStable(Position(1, 0)));

  // 柵の中に建設できる
  auto pos1 = Position(0, 3);
  EXPECT_TRUE(farm.buildStable(pos1));
  // PASTUREとSTABLEの両方のフラグが立っていることを確認
  EXPECT_EQ(farm.getField(pos1).getType(),
            static_cast<FieldType>(static_cast<int>(FieldType::PASTURE) |
                                   static_cast<int>(FieldType::STABLE)));

  // 空の場所に建設できる
  auto pos2 = Position(2, 3);
  EXPECT_TRUE(farm.buildStable(pos2));

  std::vector<FencePosition> fences2 = {
      FencePosition::create(Position(2, 3), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(2, 3), FencePosition::Edge::BOTTOM)
          .value(),
      FencePosition::create(Position(2, 4), FencePosition::Edge::LEFT).value(),
  };
  // 厩を囲うように柵を建設できる
  EXPECT_TRUE(farm.buildFence(fences2));
  // PASTUREとSTABLEの両方のフラグが立っていることを確認
  EXPECT_EQ(farm.getField(pos2).getType(),
            static_cast<FieldType>(static_cast<int>(FieldType::PASTURE) |
                                   static_cast<int>(FieldType::STABLE)));

  // 5つ以上の建設できない
  farm.buildStable(Position(0, 4));
  farm.buildStable(Position(1, 3));
  EXPECT_FALSE(farm.buildStable(Position(1, 4)));
}

TEST_F(FarmTest, LiveStock) {
  // 柵がない場合は配置できない
  std::vector<LivestockPlacement> placements = {
      LivestockPlacement{size_t(0), Resource(ResourceType::SHEEP, 10)},
      LivestockPlacement{size_t(1), Resource(ResourceType::BOAR, 10)},
  };
  EXPECT_FALSE(farm.placeLivestock(placements));

  std::vector<FencePosition> fences = {
      FencePosition::create(Position(0, 4), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(0, 4), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(1, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 4), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 3), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(2, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(2, 4), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 4), FencePosition::Edge::RIGHT).value(),
      FencePosition::create(Position(0, 4), FencePosition::Edge::RIGHT).value(),
  };
  // 有効な柵の配置
  farm.buildFence(fences);
  auto pos1 = Position(1, 3);
  farm.buildStable(pos1);

  // 家畜が多すぎて入らない
  EXPECT_FALSE(farm.placeLivestock(placements));

  std::vector<LivestockPlacement> placements2 = {
      LivestockPlacement{size_t(0), Resource(ResourceType::SHEEP, 2)},
      LivestockPlacement{size_t(1), Resource(ResourceType::BOAR, 7)},
  };
  // ちょうど入る
  EXPECT_TRUE(farm.placeLivestock(placements2));
}

TEST_F(FarmTest, BuildRooms) {
  // 木の部屋を建設
  auto pos1 = Position(0, 0);
  EXPECT_TRUE(farm.buildRoom(pos1, RoomType::WOOD));
  EXPECT_EQ(farm.getField(pos1).getType(), FieldType::WOOD_ROOM);
  // 孤立した場所には建設できない
  EXPECT_FALSE(farm.buildRoom(Position(0, 4), RoomType::WOOD));
  // 木の家の時は粘土の家は建設できない
  EXPECT_FALSE(farm.buildRoom(Position(2, 1), RoomType::CLAY));
  // 木の家の時は石の家は建設できない
  EXPECT_FALSE(farm.buildRoom(Position(2, 1), RoomType::STONE));
}

TEST_F(FarmTest, LiveStockSpecialCases) {
  // 左下のマスに2匹は配置できない
  std::vector<LivestockPlacement> tooMany = {
      LivestockPlacement{Position{2, 0}, Resource(ResourceType::SHEEP, 2)},
  };
  EXPECT_FALSE(farm.placeLivestock(tooMany));

  // 左下のマスに1匹配置
  std::vector<LivestockPlacement> placements = {
      LivestockPlacement{Position{2, 0}, Resource(ResourceType::SHEEP, 1)},
  };
  EXPECT_TRUE(farm.placeLivestock(placements));

  // 囲われていない厩に1匹配置
  auto pos1 = Position(1, 1);
  farm.buildStable(pos1);  // 柵で囲われていない位置に厩を建設
  // 囲われていない厩に2匹は配置できない
  std::vector<LivestockPlacement> tooManyStable = {
      LivestockPlacement{Position{1, 1}, Resource(ResourceType::BOAR, 2)},
  };
  EXPECT_FALSE(farm.placeLivestock(tooManyStable));

  std::vector<LivestockPlacement> stablePlacement = {
      LivestockPlacement{Position{1, 1}, Resource(ResourceType::BOAR, 1)},
  };
  EXPECT_TRUE(farm.placeLivestock(stablePlacement));
}