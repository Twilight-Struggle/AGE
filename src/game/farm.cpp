#include "game/farm.hpp"

Farm::Farm() : numRooms(2), numStables(0), roomType(RoomType::WOOD) {
    // 3x5のグリッドを初期化
    fields.resize(3, std::vector<Field>(5));
}

bool Farm::canPlace(int x, int y) const {
    // 座標が有効範囲内かチェック
    return x >= 0 && x < 3 && y >= 0 && y < 5 && fields[x][y].getType() == FieldType::EMPTY;
}

bool Farm::isFenceAt(int x, int y, FencePosition::Edge edge) const {
    return std::find_if(fences.begin(), fences.end(),
        [x, y, edge](const FencePosition& pos) {
            return pos.x == x && pos.y == y && pos.edge == edge;
        }) != fences.end();
}

std::vector<std::vector<bool>> Farm::getEnclosedAreas() const {
    std::vector<std::vector<bool>> enclosed(3, std::vector<bool>(5, false));
    // フラッドフィルアルゴリズムで囲われた領域を特定
    // 柵で区切られていない隣接マスに再帰的に移動
    // ...
    return enclosed;
}

bool Farm::isValidEnclosure(const std::vector<std::vector<bool>>& newEnclosure) const {
    // 新しい囲いが有効かチェック
    // 1. すべての囲われたマスが完全に柵で囲まれているか
    // 2. 既存の牧場を適切に分割または拡張しているか
    // ...
    return true;
}

bool Farm::canBuildFence(const std::vector<FencePosition>& newfences) const {
    // 既存の柵と新しい柵の合計が15個以下であることを確認
    if (fences.size() + newfences.size() > 15) return false;

    // 基本的なバリデーション
    for (const auto& fence : newfences) {
        if (fence.x < 0 || fence.x >= 3 || fence.y < 0 || fence.y >= 5) return false;
        if (isFenceAt(fence.x, fence.y, fence.edge)) return false;
    }

    // すべての柵を一時的に追加してシミュレーション
    std::vector<FencePosition> tempFences = fences;
    tempFences.insert(tempFences.end(), newfences.begin(), newfences.end());

    // 新しい囲いの正当性をチェック
    auto newEnclosure = getEnclosedAreas();
    return isValidEnclosure(newEnclosure);
}

void Farm::buildFence(const std::vector<FencePosition>& newfences) {
    if (!canBuildFence(newfences)) return;
    
    // すべての柵を一度に追加
    fences.insert(fences.end(), newfences.begin(), newfences.end());
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

void Farm::plowField(int x, int y) {
    if (!canPlowField(x, y)) return;
    fields[x][y].plow();
} 