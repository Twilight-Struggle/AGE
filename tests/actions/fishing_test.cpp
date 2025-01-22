#include "game/actions/fishing.hpp"

#include <gtest/gtest.h>

class FishingTest : public ::testing::Test {
 protected:
  Player player{1};
  Fishing action;
};

TEST_F(FishingTest, ExecuteGivesOneFood) {
  const int initial_resource =
      player.getResource(ResourceType::FOOD).getAmount();

  action.roundStart();
  const auto args = NoArgs{};
  const bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(),
            initial_resource + 1);
}

TEST_F(FishingTest, GetActionTypeReturnsFishing) {
  EXPECT_EQ(action.getActionType(), ActionType::FISHING);
}

TEST_F(FishingTest, CannotExecuteMultipleTimes) {
  const int initial_resource =
      player.getResource(ResourceType::FOOD).getAmount();
  action.roundStart();

  const auto args = NoArgs{};
  action.execute(player, args);
  const bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(),
            initial_resource + 1);
}
