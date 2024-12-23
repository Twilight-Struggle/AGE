#include <gtest/gtest.h>
#include "game/Field.hpp"

class FieldTest : public ::testing::Test {
protected:
    void SetUp() override {
        field = new Field();
    }

    void TearDown() override {
        delete field;
    }

    Field* field;
};

// 初期状態のテスト
TEST_F(FieldTest, InitialState) {
    EXPECT_EQ(field->getType(), FieldType::EMPTY);
}

// 畑を耕すテスト
TEST_F(FieldTest, PlowField) {
    field->plow();
    EXPECT_EQ(field->getType(), FieldType::FIELD);
}

// 柵を建てるテスト
TEST_F(FieldTest, BuildFence) {
    field->fence();
    EXPECT_EQ(field->getType(), FieldType::PASTURE);
}

// 部屋を建てるテスト
TEST_F(FieldTest, BuildRooms) {
    field->buildWoodRoom();
    EXPECT_EQ(field->getType(), FieldType::WOOD_ROOM);

    field->buildClayRoom();
    EXPECT_EQ(field->getType(), FieldType::CLAY_ROOM);

    field->buildStoneRoom();
    EXPECT_EQ(field->getType(), FieldType::STONE_ROOM);
}

// リソースの追加テスト
TEST_F(FieldTest, AddContent) {
    // 畑に作物を追加
    field->plow();
    Resource grain(ResourceType::GRAIN, 1);
    EXPECT_TRUE(field->addContent(grain));
    
    // 同じタイプの作物を追加
    Resource moreGrain(ResourceType::GRAIN, 2);
    EXPECT_TRUE(field->addContent(moreGrain));
    
    // 畑に家畜を追加しようとする（失敗するはず）
    Resource sheep(ResourceType::SHEEP, 1);
    EXPECT_FALSE(field->addContent(sheep));

    // 牧場に家畜を追加
    Field pasture;
    pasture.fence();
    EXPECT_TRUE(pasture.addContent(sheep));
    
    // 牧場に作物を追加しようとする（失敗するはず）
    EXPECT_FALSE(pasture.addContent(grain));
}

// リソースの削除テスト
TEST_F(FieldTest, RemoveContent) {
    field->plow();
    Resource grain(ResourceType::GRAIN, 3);
    field->addContent(grain);
    
    // 一部を削除
    EXPECT_TRUE(field->removeContent(Resource(ResourceType::GRAIN, 1)));
    
    // 存在しない量を削除しようとする
    EXPECT_FALSE(field->removeContent(Resource(ResourceType::GRAIN, 3)));
    
    // 存在しないタイプを削除しようとする
    EXPECT_FALSE(field->removeContent(Resource(ResourceType::SHEEP, 1)));
    
    // 残りを全て削除
    EXPECT_TRUE(field->removeContent(Resource(ResourceType::GRAIN, 2)));
}

// 複数のリソースを扱うテスト
TEST_F(FieldTest, MultipleResources) {
    // 牧場を作成して家畜を追加
    field->fence();
    field->addContent(Resource(ResourceType::SHEEP, 2));
    
    // 別の畑を作成して穀物を追加
    Field anotherField;
    anotherField.plow();
    anotherField.addContent(Resource(ResourceType::GRAIN, 3));
    
    // 羊を1匹減らす
    EXPECT_TRUE(field->removeContent(Resource(ResourceType::SHEEP, 1)));
    
    // 穀物が入っている畑に野菜を追加しようとする（失敗するはず）
    EXPECT_FALSE(anotherField.addContent(Resource(ResourceType::VEGETABLE, 1)));
    
    // 穀物を全て減らす
    EXPECT_TRUE(anotherField.removeContent(Resource(ResourceType::GRAIN, 3)));
}