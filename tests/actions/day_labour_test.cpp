#include "game/actions/day_labour.hpp"

#include <gtest/gtest.h>

class DayLabourTest : public ::testing::Test {
 protected:
  Player player{1};
  DayLabour action;
};

TEST_F(DayLabourTest, ExecuteGivesTwoFood) {
  const int initial_food = player.getResource(ResourceType::FOOD).getAmount();

  const auto args = NoArgs{};
  const bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(),
            initial_food + 2);
}

TEST_F(DayLabourTest, GetActionTypeReturnsDayLabour) {
  EXPECT_EQ(action.getActionType(), ActionType::DAY_LABOUR);
}

TEST_F(DayLabourTest, CanExecuteMultipleTimes) {
  const int initial_food = player.getResource(ResourceType::FOOD).getAmount();

  const auto args = NoArgs{};
  action.execute(player, args);
  const bool second_result = action.execute(player, args);

  EXPECT_TRUE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(),
            initial_food + 4);  // 合計4食料増えている
}
