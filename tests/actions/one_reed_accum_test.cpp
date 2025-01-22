#include "game/actions/one_reed_accum.hpp"

#include <gtest/gtest.h>

class OneReedAccumTest : public ::testing::Test {
 protected:
  Player player{1};
  OneReedAccum action;
};

TEST_F(OneReedAccumTest, ExecuteGivesOneReed) {
  int initial_resource = player.getResource(ResourceType::REED).getAmount();

  action.roundStart();
  auto args = NoArgs{};
  bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::REED).getAmount(),
            initial_resource + 1);
}

TEST_F(OneReedAccumTest, GetActionTypeReturnsOneReedAccum) {
  EXPECT_EQ(action.getActionType(), ActionType::ONE_REED_ACCUM);
}

// 複数回実行できるないことを確認
TEST_F(OneReedAccumTest, CannotExecuteMultipleTimes) {
  int initial_resource = player.getResource(ResourceType::REED).getAmount();
  action.roundStart();

  // 2回実行
  auto args = NoArgs{};
  action.execute(player, args);
  bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::REED).getAmount(),
            initial_resource + 1);
}
