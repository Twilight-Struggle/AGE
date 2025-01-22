#include "game/actions/gain_grain.hpp"

#include <gtest/gtest.h>

class GainGrainTest : public ::testing::Test {
 protected:
  Player player{1};  // プレイヤーID: 1
  GainGrain action;
};

TEST_F(GainGrainTest, ExecuteGivesOneGrain) {
  int initial_grain = player.getResource(ResourceType::GRAIN).getAmount();

  auto args = NoArgs{};
  bool result = action.execute(player, args);

  EXPECT_TRUE(result);
  EXPECT_EQ(player.getResource(ResourceType::GRAIN).getAmount(),
            initial_grain + 1);
}

TEST_F(GainGrainTest, GetActionTypeReturnsGrain) {
  EXPECT_EQ(action.getActionType(), ActionType::GAIN_GRAIN);
}

// 複数回実行できることを確認するテスト
TEST_F(GainGrainTest, CanExecuteMultipleTimes) {
  int initial_grain = player.getResource(ResourceType::GRAIN).getAmount();

  // 2回実行
  auto args = NoArgs{};
  action.execute(player, args);
  bool second_result = action.execute(player, args);

  EXPECT_TRUE(second_result);
  EXPECT_EQ(player.getResource(ResourceType::GRAIN).getAmount(),
            initial_grain + 2);
}
