#include "game/actions/one_clay_accum.hpp"

#include <gtest/gtest.h>

class OneClayAccumTest : public ::testing::Test {
 protected:
  Player player{1};
  OneClayAccum action;
};

TEST_F(OneClayAccumTest, ExecuteGivesOneClay) {
  const int initial_resource =
      player.getResource(ResourceType::CLAY).getAmount();

  action.roundStart();
  const auto args = NoArgs{};
  const bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::CLAY).getAmount(),
            initial_resource + 1);
}

TEST_F(OneClayAccumTest, GetActionTypeReturnsOneClayAccum) {
  EXPECT_EQ(action.getActionType(), ActionType::ONE_CLAY_ACCUM);
}

TEST_F(OneClayAccumTest, CannotExecuteMultipleTimes) {
  const int initial_resource =
      player.getResource(ResourceType::CLAY).getAmount();
  action.roundStart();

  const auto args = NoArgs{};
  action.execute(player, args);
  const bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::CLAY).getAmount(),
            initial_resource + 1);
}
