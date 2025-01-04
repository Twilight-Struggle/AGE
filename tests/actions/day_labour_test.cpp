#include <gtest/gtest.h>
#include "game/actions/day_labour.hpp"

class DayLabourTest : public ::testing::Test {
protected:
    Player player{1};  // プレイヤーID: 1
    DayLabour action;
};

TEST_F(DayLabourTest, ExecuteGivesTwoFood) {
    int initial_food = player.getResource(ResourceType::FOOD).getAmount();
    
    bool result = action.execute(player);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(), initial_food + 2);
}

TEST_F(DayLabourTest, RoundStartDoesNothing) {
    int initial_food = player.getResource(ResourceType::FOOD).getAmount();
    action.roundStart();
    EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(), initial_food);
}

TEST_F(DayLabourTest, GetActionTypeReturnsDayLabour) {
    EXPECT_EQ(action.getActionType(), ActionType::DAY_LABOUR);
}

// 複数回実行できることを確認するテスト
TEST_F(DayLabourTest, CanExecuteMultipleTimes) {
    int initial_food = player.getResource(ResourceType::FOOD).getAmount();
    
    // 2回実行
    action.execute(player);
    bool second_result = action.execute(player);
    
    EXPECT_TRUE(second_result);
    EXPECT_EQ(player.getResource(ResourceType::FOOD).getAmount(), initial_food + 4);  // 合計4食料増えている
}

