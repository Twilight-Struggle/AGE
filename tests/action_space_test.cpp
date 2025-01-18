#include "game/action_space.hpp"

#include <gtest/gtest.h>

#include "game/actions/day_labour.hpp"

class ActionSpaceTest : public ::testing::Test {
 protected:
  ActionSpace actionSpace{std::make_unique<DayLabour>()};
  Player player{1};
  Player player2{2};
};

TEST_F(ActionSpaceTest, Unoccupied) { EXPECT_FALSE(actionSpace.isOccupied()); }

TEST_F(ActionSpaceTest, OccupiedAfterAction) {
  auto args = DayLabourArgs{};
  EXPECT_TRUE(actionSpace.execute(player, args));
  EXPECT_TRUE(actionSpace.isOccupied());
}

TEST_F(ActionSpaceTest, OccupiedSpaceCannotBeExecutedAgain) {
  auto args = DayLabourArgs{};
  EXPECT_TRUE(actionSpace.execute(player, args));
  EXPECT_FALSE(actionSpace.execute(player2, args));
}

TEST_F(ActionSpaceTest, ResetAfterExecution) {
  auto args = DayLabourArgs{};
  EXPECT_TRUE(actionSpace.execute(player, args));
  actionSpace.reset();
  EXPECT_FALSE(actionSpace.isOccupied());
  EXPECT_TRUE(actionSpace.execute(player, args));
}

TEST_F(ActionSpaceTest, OccupiedByPlayer) {
  auto args = DayLabourArgs{};
  EXPECT_TRUE(actionSpace.execute(player, args));
  EXPECT_TRUE(actionSpace.isOccupiedByPlayer(1));
  EXPECT_FALSE(actionSpace.isOccupiedByPlayer(2));
}
