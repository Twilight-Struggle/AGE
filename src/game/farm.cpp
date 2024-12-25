#include <functional>
#include "game/farm.hpp"

Farm::Farm() : numRooms(2), numStables(0), roomType(RoomType::WOOD) {
    // 3x5のグリッドを初期化
    fields.resize(3, std::vector<Field>(5));
    fields[2][0].buildWoodRoom();
    fields[1][0].buildWoodRoom();
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

std::vector<std::vector<bool>> Farm::getEnclosedAreas(const std::vector<FencePosition>& tempFences) const {
    std::vector<std::vector<bool>> enclosed(3, std::vector<bool>(5, false));
    std::vector<std::vector<bool>> visited(3, std::vector<bool>(5, false));

    // フラッドフィルのヘルパー関数
    std::function<void(int, int, bool)> floodFill = [&](int x, int y, bool isEnclosed) {
        if (x < 0 || x >= 3 || y < 0 || y >= 5 || visited[x][y]) {
            return;
        }

        visited[x][y] = true;
        enclosed[x][y] = isEnclosed;

        // 上下左右の隣接マスをチェック
        const std::pair<int, int> directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        const FencePosition::Edge edges[4] = {
            FencePosition::Edge::TOP,
            FencePosition::Edge::BOTTOM, 
            FencePosition::Edge::LEFT,
            FencePosition::Edge::RIGHT
        };

        for (int i = 0; i < 4; i++) {
            int nx = x + directions[i].first;
            int ny = y + directions[i].second;
            
            // 柵があるかチェック
            bool hasFence = std::find_if(tempFences.begin(), tempFences.end(),
                [x, y, edge = edges[i]](const FencePosition& pos) {
                    return pos.x == x && pos.y == y && pos.edge == edge;
                }) != tempFences.end();

            if (!hasFence) {
                floodFill(nx, ny, isEnclosed);
            }
        }
    };

    // 外側から探索を開始
    floodFill(0, 0, false);

    // 未訪問のマスは囲われている
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (!visited[i][j]) {
                floodFill(i, j, true);
            }
        }
    }

    return enclosed;
}

bool Farm::isValidEnclosure(const std::vector<std::vector<bool>>& newEnclosure) const {
    // 囲われた領域のサイズをカウント
    int enclosedCount = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (newEnclosure[i][j]) {
                enclosedCount++;
            }
        }
    }

    // 囲われた領域が存在しない場合は無効
    if (enclosedCount == 0) {
        return false;
    }

    // 各囲われたマスが完全に柵で囲まれているか確認
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (newEnclosure[i][j]) {
                // 上下左右のいずれかが囲われていない場合、
                // そのマスとの境界に柵が必要
                if ((i > 0 && !newEnclosure[i-1][j]) ||
                    (i < 2 && !newEnclosure[i+1][j]) ||
                    (j > 0 && !newEnclosure[i][j-1]) ||
                    (j < 4 && !newEnclosure[i][j+1])) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Farm::canBuildFence(const std::vector<FencePosition>& newFences) const {
    // 既存の柵と新しい柵の合計が15個以下であることを確認
    if (fences.size() + newFences.size() > 15) return false;

    // 基本的なバリデーション
    for (const auto& fence : newFences) {
        if (fence.x < 0 || fence.x >= 3 || fence.y < 0 || fence.y >= 5) return false;
        if (isFenceAt(fence.x, fence.y, fence.edge)) return false;
        if (!canPlace(fence.x, fence.y)) return false;
    }

    // すべての柵を一時的に追加してシミュレーション
    std::vector<FencePosition> tempFences = fences;
    tempFences.insert(tempFences.end(), newFences.begin(), newFences.end());

    // 新しい囲いの正当性をチェック
    auto newEnclosure = getEnclosedAreas(tempFences);
    return isValidEnclosure(newEnclosure);
}

bool Farm::buildFence(const std::vector<FencePosition>& newFences) {
    if (!canBuildFence(newFences)) return false;
    
    // すべての柵を一度に追加
    fences.insert(fences.end(), newFences.begin(), newFences.end());
    return true;
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

bool Farm::buildRoom(int x, int y, RoomType roomType) {
    if (!canBuildRoom(x, y, roomType)) return false;
    if (roomType == RoomType::WOOD) {
        fields[x][y].buildWoodRoom();
    } else if (roomType == RoomType::CLAY) {
        fields[x][y].buildClayRoom();
    } else if (roomType == RoomType::STONE) {
        fields[x][y].buildStoneRoom();
    }
    numRooms++;
    return true;
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

bool Farm::plowField(int x, int y) {
    if (!canPlowField(x, y)) return false;
    fields[x][y].plow();
    return true;
} 
