#include "game/actions/four_woods_accum.hpp"

#include <gtest/gtest.h>

class FourWoodsAccumTest : public ::testing::Test {
 protected:
  Player player{1};
  FourWoodsAccum action;
};

TEST_F(FourWoodsAccumTest, ExecuteGivesFourWood) {
  int initial_wood = player.getResource(ResourceType::WOOD).getAmount();

  action.roundStart();
  auto args = NoArgs{};
  bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(),
            initial_wood + 4);
}

TEST_F(FourWoodsAccumTest, GetActionTypeReturnsFourWoodsAccum) {
  EXPECT_EQ(action.getActionType(), ActionType::FOUR_WOODS_ACCUM);
}

// 複数回実行できるないことを確認
TEST_F(FourWoodsAccumTest, CannotExecuteMultipleTimes) {
  int initial_wood = player.getResource(ResourceType::WOOD).getAmount();
  action.roundStart();

  // 2回実行
  auto args = NoArgs{};
  action.execute(player, args);
  bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::WOOD).getAmount(),
            initial_wood + 4);
}
