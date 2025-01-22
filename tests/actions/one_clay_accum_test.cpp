#include "game/actions/one_clay_accum.hpp"

#include <gtest/gtest.h>

class OneClayAccumTest : public ::testing::Test {
 protected:
  Player player{1};
  OneClayAccum action;
};

TEST_F(OneClayAccumTest, ExecuteGivesOneClay) {
  int initial_resource = player.getResource(ResourceType::CLAY).getAmount();

  action.roundStart();
  auto args = NoArgs{};
  bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::CLAY).getAmount(),
            initial_resource + 1);
}

TEST_F(OneClayAccumTest, GetActionTypeReturnsOneClayAccum) {
  EXPECT_EQ(action.getActionType(), ActionType::ONE_CLAY_ACCUM);
}

// 複数回実行できるないことを確認
TEST_F(OneClayAccumTest, CannotExecuteMultipleTimes) {
  int initial_resource = player.getResource(ResourceType::CLAY).getAmount();
  action.roundStart();

  // 2回実行
  auto args = NoArgs{};
  action.execute(player, args);
  bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::CLAY).getAmount(),
            initial_resource + 1);
}
