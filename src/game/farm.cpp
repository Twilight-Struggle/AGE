#include "game/farm.hpp"

#include <functional>
#include <queue>

Farm::Farm() : numRooms(2), numStables(0), roomType(RoomType::WOOD) {
  // 3x5のグリッドを初期化
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 5; y++) {
      fields[Position(x, y)] = Field();
    }
  }

  // 初期の木の部屋を建設
  fields[Position(1, 0)].buildWoodRoom();
  fields[Position(2, 0)].buildWoodRoom();
}

bool Farm::canPlace(int x, int y) const {
  Position pos(x, y);
  // 座標が有効範囲内かチェック
  return x >= 0 && x < 3 && y >= 0 && y < 5 &&
         fields.at(pos).getType() == FieldType::EMPTY;
}

bool Farm::isFenceAt(int x, int y, FencePosition::Edge edge) const {
  return std::find_if(fences.begin(), fences.end(),
                      [x, y, edge](const FencePosition& pos) {
                        return pos.getX() == x && pos.getY() == y &&
                               pos.getEdge() == edge;
                      }) != fences.end();
}

std::vector<std::set<Position>> Farm::getEnclosedAreas(
    const std::set<FencePosition>& fences) const {
  std::vector<std::set<Position>> enclosures;
  std::set<Position> visited;

  // 各マスについて
  for (int x = 0; x < Position::WIDTH; x++) {
    for (int y = 0; y < Position::HEIGHT; y++) {
      Position pos(x, y);
      // すでに訪問済みの場合はスキップ
      if (visited.find(pos) != visited.end()) continue;

      // 囲まれた領域を探索
      auto [hasEnclosedPath, area] = isEnclosed(pos, fences);
      visited.insert(area.begin(), area.end());
      if (hasEnclosedPath) {
        enclosures.push_back(area);
      }
    }
  }

  return enclosures;
}

std::pair<bool, std::set<Position>> Farm::isEnclosed(
    const Position& start, const std::set<FencePosition>& fences) const {
  // スタート地点のチェックを追加
  if (fields.at(start).getType() != FieldType::EMPTY &&
      fields.at(start).getType() != FieldType::STABLE &&
      fields.at(start).getType() != FieldType::PASTURE &&
      fields.at(start).getType() !=
          static_cast<FieldType>(static_cast<int>(FieldType::STABLE) |
                                 static_cast<int>(FieldType::PASTURE))) {
    return {false, std::set<Position>()};
  }

  std::queue<Position> queue;
  std::set<Position> visited;
  std::set<Position> area;
  bool hasEnclosedPath = true;  // 領域全体として囲まれているかのフラグ

  queue.push(start);
  visited.insert(start);

  while (!queue.empty()) {
    Position current = queue.front();
    queue.pop();
    area.insert(current);  // 一旦エリアに追加

    // 4方向をチェック
    for (const auto& dir : {
             std::make_pair(0, -1),  // 左
             std::make_pair(1, 0),   // 下
             std::make_pair(0, 1),   // 右
             std::make_pair(-1, 0)   // 上
         }) {
      Position next(current.x + dir.first, current.y + dir.second);

      // 柵の確認を先に行う
      FencePosition::Edge edge;
      std::optional<FencePosition> fenceOpt;
      if (dir.first == 0 && dir.second == -1) {
        // 左方向：現在のマスのLEFT
        edge = FencePosition::Edge::LEFT;
        fenceOpt = FencePosition::create(current.x, current.y, edge);
      } else if (dir.first == 1 && dir.second == 0) {
        // 下方向：
        if (current.x == 2) {
          // x=2の場合は現在のマスのBOTTOM
          edge = FencePosition::Edge::BOTTOM;
          fenceOpt = FencePosition::create(current.x, current.y, edge);
        } else {
          // それ以外は下のマスのTOP
          edge = FencePosition::Edge::TOP;
          fenceOpt = FencePosition::create(current.x + 1, current.y, edge);
        }
      } else if (dir.first == 0 && dir.second == 1) {
        // 右方向：
        if (current.y == 4) {
          // y=4の場合は現在のマスのRIGHT
          edge = FencePosition::Edge::RIGHT;
          fenceOpt = FencePosition::create(current.x, current.y, edge);
        } else {
          // それ以外は次のマスのLEFT
          edge = FencePosition::Edge::LEFT;
          fenceOpt = FencePosition::create(current.x, current.y + 1, edge);
        }
      } else {
        // 上方向：現在のマスのTOP
        edge = FencePosition::Edge::TOP;
        fenceOpt = FencePosition::create(current.x, current.y, edge);
      }

      // 柵がある場合は次の方向へ
      if (fenceOpt && fences.find(fenceOpt.value()) != fences.end()) {
        continue;
      }

      // 境界チェック（柵がない場合のみ）
      if (next.x < 0 || next.x >= Position::WIDTH || next.y < 0 ||
          next.y >= Position::HEIGHT ||
          (fields.at(Position(next.x, next.y)).getType() != FieldType::EMPTY &&
           fields.at(Position(next.x, next.y)).getType() != FieldType::STABLE &&
           fields.at(Position(next.x, next.y)).getType() !=
               FieldType::PASTURE &&
           fields.at(start).getType() !=
               static_cast<FieldType>(static_cast<int>(FieldType::STABLE) |
                                      static_cast<int>(FieldType::PASTURE)))) {
        hasEnclosedPath = false;  // 囲まれていない
        continue;
      }

      // 訪問していなければキューに追加
      if (visited.find(next) == visited.end()) {
        queue.push(next);
        visited.insert(next);
      }
    }
  }

  return {hasEnclosedPath, area};
}

std::pair<bool, std::vector<std::set<Position>>> Farm::isValidEnclosure(
    const std::vector<std::set<Position>>& newEnclosure,
    const std::set<FencePosition>& tempFences) const {
  // 囲まれた領域がない場合は false
  if (newEnclosure.empty()) return {false, std::vector<std::set<Position>>()};

  // 複数の領域がある場合、連結性をチェック
  if (newEnclosure.size() > 1) {
    // 領域の連結グラフを作成
    std::vector<std::vector<bool>> adjacency(
        newEnclosure.size(), std::vector<bool>(newEnclosure.size(), false));

    // 各領域ペアについて隣接関係をチェック
    for (size_t i = 0; i < newEnclosure.size(); ++i) {
      for (size_t j = i + 1; j < newEnclosure.size(); ++j) {
        bool isAdjacent = false;
        for (const auto& pos1 : newEnclosure[i]) {
          for (const auto& pos2 : newEnclosure[j]) {
            // 縦横の隣接をチェック（斜めは除外）
            if ((std::abs(pos1.x - pos2.x) == 1 && pos1.y == pos2.y) ||
                (pos1.x == pos2.x && std::abs(pos1.y - pos2.y) == 1)) {
              isAdjacent = true;
              adjacency[i][j] = adjacency[j][i] = true;
              break;
            }
          }
          if (isAdjacent) break;
        }
      }
    }

    // DFSで連結性をチェック
    std::vector<bool> visited(newEnclosure.size(), false);
    std::function<void(int)> dfs = [&](int v) {
      visited[v] = true;
      for (size_t u = 0; u < newEnclosure.size(); ++u) {
        if (adjacency[v][u] && !visited[u]) {
          dfs(u);
        }
      }
    };

    dfs(0);  // 最初の領域から探索開始

    // すべての領域が訪問されているかチェック
    if (!std::all_of(visited.begin(), visited.end(),
                     [](bool v) { return v; })) {
      return {false,
              std::vector<
                  std::set<Position>>()};  // 連結していない領域があればfalse
    }
  }

  // 以下、既存の柵チェックコード
  std::set<FencePosition> requiredFences;
  // 各囲まれた領域について
  for (const auto& area : newEnclosure) {
    // 領域の各マスについて
    for (const auto& pos : area) {
      // 左辺
      if (pos.y == 0 || std::find(area.begin(), area.end(),
                                  Position(pos.x, pos.y - 1)) == area.end()) {
        auto fence =
            FencePosition::create(pos.x, pos.y, FencePosition::Edge::LEFT);
        if (fence) requiredFences.insert(*fence);
      }
      // 上辺
      if (pos.x == 0 || std::find(area.begin(), area.end(),
                                  Position(pos.x - 1, pos.y)) == area.end()) {
        auto fence =
            FencePosition::create(pos.x, pos.y, FencePosition::Edge::TOP);
        if (fence) requiredFences.insert(*fence);
      }
      // 右辺
      if (pos.y == 4 || std::find(area.begin(), area.end(),
                                  Position(pos.x, pos.y + 1)) == area.end()) {
        if (pos.y == 4) {
          auto fence =
              FencePosition::create(pos.x, pos.y, FencePosition::Edge::RIGHT);
          if (fence) requiredFences.insert(*fence);
        } else {
          auto fence = FencePosition::create(pos.x, pos.y + 1,
                                             FencePosition::Edge::LEFT);
          if (fence) requiredFences.insert(*fence);
        }
      }
      // 下辺
      if (pos.x == 2 || std::find(area.begin(), area.end(),
                                  Position(pos.x + 1, pos.y)) == area.end()) {
        if (pos.x == 2) {
          auto fence =
              FencePosition::create(pos.x, pos.y, FencePosition::Edge::BOTTOM);
          if (fence) requiredFences.insert(*fence);
        } else {
          auto fence =
              FencePosition::create(pos.x + 1, pos.y, FencePosition::Edge::TOP);
          if (fence) requiredFences.insert(*fence);
        }
      }
    }
  }

  // 必要な柵の数と実際の柵の数が一致するかチェック
  return {requiredFences == tempFences, newEnclosure};
}

std::pair<bool, std::vector<std::set<Position>>> Farm::canBuildFence(
    const std::vector<FencePosition>& newFences) const {
  // 既存の柵と新しい柵の合計が15個以下であることを確認
  if (fences.size() + newFences.size() > 15)
    return {false, std::vector<std::set<Position>>()};

  // 基本的なバリデーション
  for (const auto& fence : newFences) {
    if (fence.getX() < 0 || fence.getX() >= 3 || fence.getY() < 0 ||
        fence.getY() >= 5)
      return {false, std::vector<std::set<Position>>()};
    // 既存の柵と重複している場合はfalse
    if (isFenceAt(fence.getX(), fence.getY(), fence.getEdge()))
      return {false, std::vector<std::set<Position>>()};
  }

  // すべての柵を一時的に追加してシミュレーション
  std::set<FencePosition> tempFences(fences.begin(), fences.end());
  tempFences.insert(newFences.begin(), newFences.end());

  // 新しい囲いの正当性をチェック
  auto newEnclosure = getEnclosedAreas(tempFences);
  auto [isValid, returnedEnclosure] =
      isValidEnclosure(newEnclosure, tempFences);
  return {isValid, returnedEnclosure};
}

bool Farm::buildFence(const std::vector<FencePosition>& newFences) {
  auto [isValid, returnedEnclosure] = canBuildFence(newFences);
  if (!isValid) {
    return false;
  }

  // すべての柵を一度に追加
  fences.insert(fences.end(), newFences.begin(), newFences.end());
  for (const auto& enclosure : returnedEnclosure) {
    for (const auto& pos : enclosure) {
      fields[pos].fence();
    }
  }
  enclosures = returnedEnclosure;
  return true;
}

bool Farm::canBuildRoom(int x, int y, RoomType roomType) const {
  if (this->roomType != roomType) return false;
  if (!canPlace(x, y)) return false;

  // 上下左右に隣接するマスをチェック
  bool hasAdjacentRoom = false;
  // 上のマス
  if (x > 0) {
    Position pos(x - 1, y);
    if (isRoom(fields.at(pos).getType())) {
      hasAdjacentRoom = true;
    }
  }
  // 下のマス
  if (x < 2) {
    Position pos(x + 1, y);
    if (isRoom(fields.at(pos).getType())) {
      hasAdjacentRoom = true;
    }
  }
  // 左のマス
  if (y > 0) {
    Position pos(x, y - 1);
    if (isRoom(fields.at(pos).getType())) {
      hasAdjacentRoom = true;
    }
  }
  // 右のマス
  if (y < 4) {
    Position pos(x, y + 1);
    if (isRoom(fields.at(pos).getType())) {
      hasAdjacentRoom = true;
    }
  }
  return hasAdjacentRoom;
}

bool Farm::buildRoom(int x, int y, RoomType roomType) {
  if (!canBuildRoom(x, y, roomType)) return false;

  Position pos(x, y);
  if (roomType == RoomType::WOOD) {
    fields[pos].buildWoodRoom();
  } else if (roomType == RoomType::CLAY) {
    fields[pos].buildClayRoom();
  } else if (roomType == RoomType::STONE) {
    fields[pos].buildStoneRoom();
  }
  numRooms++;
  return true;
}

bool Farm::canPlowField(int x, int y) const {
  if (!canPlace(x, y)) return false;

  // まずフィールド内にFIELDが存在するかチェック
  bool hasAnyField =
      std::any_of(fields.begin(), fields.end(), [](const auto& field_pair) {
        return field_pair.second.getType() == FieldType::FIELD;
      });
  // FIELDが一つも無い場合はtrueを返す
  if (!hasAnyField) return true;

  // 上下左右に隣接するマスをチェック
  bool hasAdjacentField = false;
  // 上のマス
  if (x > 0) {
    Position pos(x - 1, y);
    if (fields.at(pos).getType() == FieldType::FIELD) {
      hasAdjacentField = true;
    }
  }
  // 下のマス
  if (x < 2) {
    Position pos(x + 1, y);
    if (fields.at(pos).getType() == FieldType::FIELD) {
      hasAdjacentField = true;
    }
  }
  // 左のマス
  if (y > 0) {
    Position pos(x, y - 1);
    if (fields.at(pos).getType() == FieldType::FIELD) {
      hasAdjacentField = true;
    }
  }
  // 右のマス
  if (y < 4) {
    Position pos(x, y + 1);
    if (fields.at(pos).getType() == FieldType::FIELD) {
      hasAdjacentField = true;
    }
  }
  return hasAdjacentField;
}

bool Farm::plowField(int x, int y) {
  if (!canPlowField(x, y)) return false;
  fields[Position(x, y)].plow();
  return true;
}

bool Farm::canBuildStable(int x, int y) const {
  Position pos(x, y);
  // 座標が有効範囲内かチェック
  return x >= 0 && x < 3 && y >= 0 && y < 5 &&
         (fields.at(pos).getType() == FieldType::EMPTY ||
          fields.at(pos).getType() == FieldType::PASTURE);
}

bool Farm::buildStable(int x, int y) {
  if (!canBuildStable(x, y)) return false;
  if (numStables >= 4) return false;
  fields[Position(x, y)].stable();
  numStables++;
  return true;
}