#include "game/actions/three_woods_accum.hpp"

#include <gtest/gtest.h>

class ThreeWoodsAccumTest : public ::testing::Test {
 protected:
  Player player{1};
  ThreeWoodsAccum action;
};

TEST_F(ThreeWoodsAccumTest, ExecuteGivesThreeWood) {
  const int initial_wood = player.getResource(ResourceType::WOOD).getAmount();

  action.roundStart();
  const auto args = NoArgs{};
  const bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(),
            initial_wood + 3);
}

TEST_F(ThreeWoodsAccumTest, GetActionTypeReturnsThreeWoodsAccum) {
  EXPECT_EQ(action.getActionType(), ActionType::THREE_WOODS_ACCUM);
}

TEST_F(ThreeWoodsAccumTest, CannotExecuteMultipleTimes) {
  const int initial_wood = player.getResource(ResourceType::WOOD).getAmount();
  action.roundStart();

  const auto args = NoArgs{};
  action.execute(player, args);
  const bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(),
            initial_wood + 3);
}
