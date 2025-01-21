#include "game/actions/plow_field.hpp"

#include <gtest/gtest.h>

class PlowFieldTest : public ::testing::Test {
 protected:
  Player player{1};  // プレイヤーID: 1
  PlowField action;
};

TEST_F(PlowFieldTest, ExecutePlowsField) {
  auto args = PlowFieldArgs{{Position(0, 2), Position(0, 3)}};
  bool result = action.execute(player, args);
  EXPECT_TRUE(result);

  auto args1 = PlowFieldArgs{{Position(0, 1)}};
  bool result1 = action.execute(player, args1);
  EXPECT_TRUE(result1);

  auto args2 = PlowFieldArgs{{Position(2, 1), Position(2, 2)}};
  bool result2 = action.execute(player, args2);
  EXPECT_FALSE(result2);

  auto& farm = player.getFarm();
  EXPECT_EQ(farm.getField(Position(0, 1)).getType(), FieldType::FIELD);
  EXPECT_EQ(farm.getField(Position(0, 2)).getType(), FieldType::FIELD);
  EXPECT_EQ(farm.getField(Position(0, 3)).getType(), FieldType::FIELD);
}

TEST_F(PlowFieldTest, RoundStartDoesNothing) { action.roundStart(); }

TEST_F(PlowFieldTest, GetActionTypeReturnsDayLabour) {
  EXPECT_EQ(action.getActionType(), ActionType::PLOW_FIELD);
}