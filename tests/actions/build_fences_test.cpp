#include "game/actions/build_fences.hpp"

#include <gtest/gtest.h>

#include "game/actions/action_args.hpp"

class BuildFencesTest : public ::testing::Test {
 protected:
  Player player{1};  // プレイヤーID: 1
  BuildFences action;
};

TEST_F(BuildFencesTest, ExecuteBuildsFences) {
  const auto args = BuildFenceArgs{{
      FencePosition::create(Position(0, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(0, 3), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(1, 3), FencePosition::Edge::LEFT).value(),
      FencePosition::create(Position(2, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(2, 4), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 4), FencePosition::Edge::RIGHT).value(),
      FencePosition::create(Position(0, 4), FencePosition::Edge::RIGHT).value(),
      FencePosition::create(Position(0, 4), FencePosition::Edge::TOP).value(),
  }};
  const bool result = action.execute(player, args);
  EXPECT_TRUE(result);

  const auto args1 = BuildFenceArgs{
      {FencePosition::create(Position(1, 3), FencePosition::Edge::TOP)
           .value()}};
  const bool result1 = action.execute(player, args1);
  EXPECT_FALSE(result1);

  const auto args2 = BuildFenceArgs{{
      FencePosition::create(Position(1, 3), FencePosition::Edge::TOP).value(),
      FencePosition::create(Position(1, 4), FencePosition::Edge::TOP).value(),
  }};
  const bool result2 = action.execute(player, args2);
  EXPECT_TRUE(result2);

  const auto& farm = player.getFarm();
  EXPECT_EQ(farm.getField(Position(0, 3)).getType(), FieldType::PASTURE);
  EXPECT_EQ(farm.getField(Position(1, 3)).getType(), FieldType::PASTURE);
  EXPECT_EQ(farm.getField(Position(0, 4)).getType(), FieldType::PASTURE);
  EXPECT_EQ(farm.getField(Position(1, 4)).getType(), FieldType::PASTURE);
}

TEST_F(BuildFencesTest, RoundStartDoesNothing) { action.roundStart(); }

TEST_F(BuildFencesTest, GetActionTypeReturnsBuildFences) {
  EXPECT_EQ(action.getActionType(), ActionType::BUILD_FENCES);
}