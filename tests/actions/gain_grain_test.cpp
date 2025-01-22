#include "game/actions/gain_grain.hpp"

#include <gtest/gtest.h>

class GainGrainTest : public ::testing::Test {
 protected:
  Player player{1};
  GainGrain action;
};

TEST_F(GainGrainTest, ExecuteGivesOneGrain) {
  const int initial_grain = player.getResource(ResourceType::GRAIN).getAmount();

  const auto args = NoArgs{};
  const bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::GRAIN).getAmount(),
            initial_grain + 1);
}

TEST_F(GainGrainTest, GetActionTypeReturnsGrain) {
  EXPECT_EQ(action.getActionType(), ActionType::GAIN_GRAIN);
}

TEST_F(GainGrainTest, CanExecuteMultipleTimes) {
  const int initial_grain = player.getResource(ResourceType::GRAIN).getAmount();

  const auto args = NoArgs{};
  action.execute(player, args);
  const bool second_result = action.execute(player, args);

  EXPECT_TRUE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::GRAIN).getAmount(),
            initial_grain + 2);
}
