#include "game/Farm.hpp"

Farm::Farm() : numRooms(2), numStables(0), roomType(RoomType::WOOD) {
    // 3x5のグリッドを初期化
    fields.resize(3, std::vector<Field>(5));
}

bool Farm::canPlace(int x, int y) const {
    // 座標が有効範囲内かチェック
    return x >= 0 && x < 3 && y >= 0 && y < 5 && fields[x][y].getType() == FieldType::EMPTY;
}

bool Farm::canBuildRoom(int x, int y, RoomType roomType) const {
    if (this->roomType != roomType) return false;
    if (!canPlace(x, y)) return false;

    // 上下左右に隣接するマスをチェック
    bool hasAdjacentRoom = false;
    // 上のマス
    if (x > 0 && isRoom(fields[x-1][y].getType())) {
        hasAdjacentRoom = true;
    }
    // 下のマス
    if (x < 2 && isRoom(fields[x+1][y].getType())) {
        hasAdjacentRoom = true;
    }
    // 左のマス
    if (y > 0 && isRoom(fields[x][y-1].getType())) {
        hasAdjacentRoom = true;
    }
    // 右のマス
    if (y < 4 && isRoom(fields[x][y+1].getType())) {
        hasAdjacentRoom = true;
    }
    return hasAdjacentRoom;
}

bool Farm::canBuildFence(int x, int y) const {
    if (!canPlace(x, y)) return false;
    return fields[x][y].getType() == FieldType::EMPTY;
}

bool Farm::canPlowField(int x, int y) const {
    if (!canPlace(x, y)) return false;

    // まずフィールド内にFIELDが存在するかチェック
    bool hasAnyField = std::any_of(fields.begin(), fields.end(), 
        [](const auto& row) {
            return std::any_of(row.begin(), row.end(),
                [](const auto& field) {
                    return field.getType() == FieldType::FIELD;
                });
        });
    // FIELDが一つも無い場合はtrueを返す
    if (!hasAnyField) return true;

    // 上下左右に隣接するマスをチェック
    bool hasAdjacentField = false;
    // 上のマス
    if (x > 0 && fields[x-1][y].getType() == FieldType::FIELD) {
        hasAdjacentField = true;
    }
    // 下のマス
    if (x < 2 && fields[x+1][y].getType() == FieldType::FIELD) {
        hasAdjacentField = true;
    }
    // 左のマス
    if (y > 0 && fields[x][y-1].getType() == FieldType::FIELD) {
        hasAdjacentField = true;
    }
    // 右のマス
    if (y < 4 && fields[x][y+1].getType() == FieldType::FIELD) {
        hasAdjacentField = true;
    }
    return hasAdjacentField;
}

void Farm::buildRoom(int x, int y, RoomType roomType) {
    if (!canBuildRoom(x, y, roomType)) return;
    if (roomType == RoomType::WOOD) {
        fields[x][y].buildWoodRoom();
    } else if (roomType == RoomType::CLAY) {
        fields[x][y].buildClayRoom();
    } else if (roomType == RoomType::STONE) {
        fields[x][y].buildStoneRoom();
    }
    numRooms++;
}

void Farm::buildFence(int x, int y) {
    if (!canBuildFence(x, y)) return;
    fields[x][y].fence();
}

void Farm::plowField(int x, int y) {
    if (!canPlowField(x, y)) return;
    fields[x][y].plow();
} 