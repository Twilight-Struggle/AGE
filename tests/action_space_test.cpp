#include <gtest/gtest.h>
#include "game/action_space.hpp"
#include "game/actions/day_labour.hpp"

class ActionSpaceTest : public ::testing::Test {
protected:
    ActionSpace actionSpace{std::make_unique<DayLabour>()};
    Player player{1};
    Player player2{2};
};

TEST_F(ActionSpaceTest, Unoccupied) {
    EXPECT_FALSE(actionSpace.isOccupied());
}

TEST_F(ActionSpaceTest, OccupiedAfterAction) {
    EXPECT_TRUE(actionSpace.execute(player));
    EXPECT_TRUE(actionSpace.isOccupied());
}

TEST_F(ActionSpaceTest, OccupiedSpaceCannotBeExecutedAgain) {
    EXPECT_TRUE(actionSpace.execute(player));
    EXPECT_FALSE(actionSpace.execute(player2));
}

TEST_F(ActionSpaceTest, ResetAfterExecution) {
    EXPECT_TRUE(actionSpace.execute(player));
    actionSpace.reset();
    EXPECT_FALSE(actionSpace.isOccupied());
    EXPECT_TRUE(actionSpace.execute(player));
}

TEST_F(ActionSpaceTest, OccupiedByPlayer) {
    EXPECT_TRUE(actionSpace.execute(player));
    EXPECT_TRUE(actionSpace.isOccupiedByPlayer(1));
    EXPECT_FALSE(actionSpace.isOccupiedByPlayer(2));
}
