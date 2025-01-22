#include "game/actions/one_reed_accum.hpp"

#include <gtest/gtest.h>

class OneReedAccumTest : public ::testing::Test {
 protected:
  Player player{1};
  OneReedAccum action;
};

TEST_F(OneReedAccumTest, ExecuteGivesOneReed) {
  const int initial_resource =
      player.getResource(ResourceType::REED).getAmount();

  action.roundStart();
  const auto args = NoArgs{};
  const bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::REED).getAmount(),
            initial_resource + 1);
}

TEST_F(OneReedAccumTest, GetActionTypeReturnsOneReedAccum) {
  EXPECT_EQ(action.getActionType(), ActionType::ONE_REED_ACCUM);
}

TEST_F(OneReedAccumTest, CannotExecuteMultipleTimes) {
  const int initial_resource =
      player.getResource(ResourceType::REED).getAmount();
  action.roundStart();

  const auto args = NoArgs{};
  action.execute(player, args);
  const bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::REED).getAmount(),
            initial_resource + 1);
}
