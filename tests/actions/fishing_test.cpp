#include "game/actions/fishing.hpp"

#include <gtest/gtest.h>

class FishingTest : public ::testing::Test {
 protected:
  Player player{1};
  Fishing action;
};

TEST_F(FishingTest, ExecuteGivesOneFood) {
  int initial_resource = player.getResource(ResourceType::FOOD).getAmount();

  action.roundStart();
  auto args = NoArgs{};
  bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(),
            initial_resource + 1);
}

TEST_F(FishingTest, GetActionTypeReturnsFishing) {
  EXPECT_EQ(action.getActionType(), ActionType::FISHING);
}

// 複数回実行できるないことを確認
TEST_F(FishingTest, CannotExecuteMultipleTimes) {
  int initial_resource = player.getResource(ResourceType::FOOD).getAmount();
  action.roundStart();

  // 2回実行
  auto args = NoArgs{};
  action.execute(player, args);
  bool second_result = action.execute(player, args);

  EXPECT_FALSE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(),
            initial_resource + 1);
}
