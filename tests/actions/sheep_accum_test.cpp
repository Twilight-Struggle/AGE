#include "game/actions/sheep_accum.hpp"

#include <gtest/gtest.h>

class SheepAccumTest : public ::testing::Test {
 protected:
  Player player{1};
  SheepAccum action;
};

TEST_F(SheepAccumTest, ExecuteGivesOneSheep) {
  int initial_resource = player.getResource(ResourceType::SHEEP).getAmount();

  action.roundStart();
  auto args = NoArgs{};
  bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::SHEEP).getAmount(),
            initial_resource + 1);
}

TEST_F(SheepAccumTest, GetActionTypeReturnsSheepAccum) {
  EXPECT_EQ(action.getActionType(), ActionType::SHEEP_ACCUM);
}

// 複数回実行できるないことを確認
TEST_F(SheepAccumTest, CannotExecuteMultipleTimes) {
  int initial_resource = player.getResource(ResourceType::SHEEP).getAmount();
  action.roundStart();

  // 2回実行
  auto args = NoArgs{};
  action.execute(player, args);
  bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::SHEEP).getAmount(),
            initial_resource + 1);
}
