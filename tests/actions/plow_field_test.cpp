#include "game/actions/plow_field.hpp"

#include <gtest/gtest.h>

class PlowFieldTest : public ::testing::Test {
 protected:
  Player player{1};
  PlowField action;
};

TEST_F(PlowFieldTest, ExecutePlowsField) {
  const auto args = PlowFieldArgs{{Position(0, 2), Position(0, 3)}};
  const bool result = action.execute(player, args);
  EXPECT_TRUE(result);

  const auto args1 = PlowFieldArgs{{Position(0, 1)}};
  const bool result1 = action.execute(player, args1);
  EXPECT_TRUE(result1);

  const auto args2 = PlowFieldArgs{{Position(2, 1), Position(2, 2)}};
  const bool result2 = action.execute(player, args2);
  EXPECT_FALSE(result2);

  const auto& farm = player.getFarm();
  EXPECT_EQ(farm.getField(Position(0, 1)).getType(), FieldType::FIELD);
  EXPECT_EQ(farm.getField(Position(0, 2)).getType(), FieldType::FIELD);
  EXPECT_EQ(farm.getField(Position(0, 3)).getType(), FieldType::FIELD);
}

TEST_F(PlowFieldTest, RoundStartDoesNothing) { action.roundStart(); }

TEST_F(PlowFieldTest, GetActionTypeReturnsPlowField) {
  EXPECT_EQ(action.getActionType(), ActionType::PLOW_FIELD);
}