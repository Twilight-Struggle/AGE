#include <gtest/gtest.h>
#include "game/farm.hpp"

class FarmTest : public ::testing::Test {
protected:
    Farm farm;
};
/*
TEST_F(FarmTest, InitialState) {
    // 初期状態で2部屋あることを確認
    EXPECT_EQ(farm.getNumRooms(), 2);
    EXPECT_EQ(farm.getNumStables(), 0);
}
*/

TEST_F(FarmTest, PlowField) {
    // 木の家の上には耕せない
    EXPECT_FALSE(farm.plowField(1, 0));
    // 最初の畑は任意の場所に作れる
    EXPECT_TRUE(farm.plowField(1, 1));
    
    // 2つ目の畑は最初の畑に隣接している必要がある
    EXPECT_TRUE(farm.plowField(1, 2));
    EXPECT_FALSE(farm.plowField(0, 0));
}

TEST_F(FarmTest, BuildFence) {
    std::vector<Farm::FencePosition> fences = {
        {0, 0, Farm::FencePosition::Edge::TOP},
        {0, 0, Farm::FencePosition::Edge::LEFT},
        {1, 0, Farm::FencePosition::Edge::LEFT},
        {1, 0, Farm::FencePosition::Edge::BOTTOM},
        {0, 1, Farm::FencePosition::Edge::TOP},
        {0, 1, Farm::FencePosition::Edge::RIGHT},
        {1, 1, Farm::FencePosition::Edge::BOTTOM},
        {1, 1, Farm::FencePosition::Edge::RIGHT},
    };
    // 空でない場所に柵はおけない
    EXPECT_FALSE(farm.buildFence(fences));
    
    std::vector<Farm::FencePosition> fences2 = {
        {0, 3, Farm::FencePosition::Edge::TOP},
        {0, 3, Farm::FencePosition::Edge::LEFT},
        {1, 3, Farm::FencePosition::Edge::LEFT},
        {1, 3, Farm::FencePosition::Edge::BOTTOM},
        {0, 4, Farm::FencePosition::Edge::TOP},
        {0, 4, Farm::FencePosition::Edge::RIGHT},
        {1, 4, Farm::FencePosition::Edge::BOTTOM},
        {1, 4, Farm::FencePosition::Edge::RIGHT},
    };
    // 有効な柵の配置
    EXPECT_TRUE(farm.buildFence(fences2));

    std::vector<Farm::FencePosition> fences3 = {
        {0, 3, Farm::FencePosition::Edge::TOP},
        {0, 3, Farm::FencePosition::Edge::LEFT},
        {1, 3, Farm::FencePosition::Edge::LEFT},
        {1, 3, Farm::FencePosition::Edge::BOTTOM},
        {0, 4, Farm::FencePosition::Edge::TOP},
        {0, 4, Farm::FencePosition::Edge::RIGHT},
        {1, 4, Farm::FencePosition::Edge::BOTTOM},
        {1, 4, Farm::FencePosition::Edge::RIGHT},
    };
    // 有効な柵の配置
    EXPECT_TRUE(farm.buildFence(fences3));
}

TEST_F(FarmTest, BuildRooms) {
    // 木の部屋を建設
    EXPECT_TRUE(farm.buildRoom(0, 0, RoomType::WOOD));
    // 孤立した場所には建設できない
    EXPECT_FALSE(farm.buildRoom(0, 4, RoomType::WOOD));
    // 木の家の時は粘土の家は建設できない
    EXPECT_FALSE(farm.buildRoom(2, 1, RoomType::CLAY));
    // 木の家の時は石の家は建設できない
    EXPECT_FALSE(farm.buildRoom(2, 1, RoomType::STONE));
} 