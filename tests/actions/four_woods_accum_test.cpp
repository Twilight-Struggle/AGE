#include "game/actions/four_woods_accum.hpp"

#include <gtest/gtest.h>

class FourWoodsAccumTest : public ::testing::Test {
 protected:
  Player player{1};
  FourWoodsAccum action;
};

TEST_F(FourWoodsAccumTest, ExecuteGivesFourWood) {
  const int initial_wood = player.getResource(ResourceType::WOOD).getAmount();

  action.roundStart();
  const auto args = NoArgs{};
  const bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(),
            initial_wood + 4);
}

TEST_F(FourWoodsAccumTest, GetActionTypeReturnsFourWoodsAccum) {
  EXPECT_EQ(action.getActionType(), ActionType::FOUR_WOODS_ACCUM);
}

TEST_F(FourWoodsAccumTest, CannotExecuteMultipleTimes) {
  const int initial_wood = player.getResource(ResourceType::WOOD).getAmount();
  action.roundStart();

  const auto args = NoArgs{};
  action.execute(player, args);
  const bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(),
            initial_wood + 4);
}
