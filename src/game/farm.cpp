#include "game/farm.hpp"

#include <cstddef>
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

bool Farm::canPlace(Position pos) const {
  // 座標が有効範囲内かチェック
  return pos.x >= 0 && pos.x < 3 && pos.y >= 0 && pos.y < 5 &&
         fields.at(pos).getType() == FieldType::EMPTY;
}

bool Farm::isFenceAt(Position pos, FencePosition::Edge edge) const {
  return std::find_if(fences.begin(), fences.end(),
                      [pos, edge](const FencePosition& fencePos) {
                        return fencePos.getPosition().x == pos.x &&
                               fencePos.getPosition().y == pos.y &&
                               fencePos.getEdge() == edge;
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
        fenceOpt = FencePosition::create(current, edge);
      } else if (dir.first == 1 && dir.second == 0) {
        // 下方向：
        if (current.x == 2) {
          // x=2の場合は現在のマスのBOTTOM
          edge = FencePosition::Edge::BOTTOM;
          fenceOpt = FencePosition::create(current, edge);
        } else {
          // それ以外は下のマスのTOP
          edge = FencePosition::Edge::TOP;
          fenceOpt = FencePosition::create(current.down(), edge);
        }
      } else if (dir.first == 0 && dir.second == 1) {
        // 右方向：
        if (current.y == 4) {
          // y=4の場合は現在のマスのRIGHT
          edge = FencePosition::Edge::RIGHT;
          fenceOpt = FencePosition::create(current, edge);
        } else {
          // それ以外は次のマスのLEFT
          edge = FencePosition::Edge::LEFT;
          fenceOpt = FencePosition::create(current.right(), edge);
        }
      } else {
        // 上方向：現在のマスのTOP
        edge = FencePosition::Edge::TOP;
        fenceOpt = FencePosition::create(current, edge);
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
        auto fence = FencePosition::create(pos, FencePosition::Edge::LEFT);
        if (fence) requiredFences.insert(*fence);
      }
      // 上辺
      if (pos.x == 0 || std::find(area.begin(), area.end(),
                                  Position(pos.x - 1, pos.y)) == area.end()) {
        auto fence = FencePosition::create(pos, FencePosition::Edge::TOP);
        if (fence) requiredFences.insert(*fence);
      }
      // 右辺
      if (pos.y == 4 ||
          std::find(area.begin(), area.end(), pos.right()) == area.end()) {
        if (pos.y == 4) {
          auto fence = FencePosition::create(pos, FencePosition::Edge::RIGHT);
          if (fence) requiredFences.insert(*fence);
        } else {
          auto fence =
              FencePosition::create(pos.right(), FencePosition::Edge::LEFT);
          if (fence) requiredFences.insert(*fence);
        }
      }
      // 下辺
      if (pos.x == 2 ||
          std::find(area.begin(), area.end(), pos.down()) == area.end()) {
        if (pos.x == 2) {
          auto fence = FencePosition::create(pos, FencePosition::Edge::BOTTOM);
          if (fence) requiredFences.insert(*fence);
        } else {
          auto fence =
              FencePosition::create(pos.down(), FencePosition::Edge::TOP);
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
    if (fence.getPosition().x < 0 || fence.getPosition().x >= 3 ||
        fence.getPosition().y < 0 || fence.getPosition().y >= 5)
      return {false, std::vector<std::set<Position>>()};
    // 既存の柵と重複している場合はfalse
    if (isFenceAt(fence.getPosition(), fence.getEdge()))
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

bool Farm::canBuildStable(Position pos) const {
  // 座標が有効範囲内かチェック
  return pos.x >= 0 && pos.x < 3 && pos.y >= 0 && pos.y < 5 &&
         (fields.at(pos).getType() == FieldType::EMPTY ||
          fields.at(pos).getType() == FieldType::PASTURE);
}

bool Farm::buildStable(Position pos) {
  if (!canBuildStable(pos)) return false;
  if (numStables >= 4) return false;
  fields[pos].stable();
  numStables++;
  return true;
}

int Farm::getStableCount(const std::set<Position>& enclosure) const {
  int count = 0;
  for (const auto& pos : enclosure) {
    const auto& field = fields.at(pos);
    if (static_cast<int>(field.getType()) &
        static_cast<int>(FieldType::STABLE)) {
      count++;
    }
  }
  return count;
}

int Farm::getMaxCapacity(size_t enclosureIndex) const {
  if (enclosureIndex >= enclosures.size()) return 0;

  const auto& enclosure = enclosures[enclosureIndex];
  int baseCapacity = enclosure.size() * 2;  // 基本は1マスあたり2匹
  int stableCount = getStableCount(enclosure);

  return baseCapacity * (1 << stableCount);  // 2^(厩の数)倍
}

bool Farm::validateLivestockPlacement(
    const std::vector<LivestockPlacement>& placements) const {
  for (const auto& placement : placements) {
    if (std::holds_alternative<size_t>(placement.location)) {
      // enclosureへの配置の検証
      size_t enclosureIndex = std::get<size_t>(placement.location);
      if (!validateEnclosurePlacement(enclosureIndex, placement.livestock)) {
        return false;
      }
    } else {
      // 特定マスへの配置の検証
      Position pos = std::get<Position>(placement.location);
      if (!validateSinglePositionPlacement(pos, placement.livestock)) {
        return false;
      }
    }
  }
  return true;
}

bool Farm::validateSinglePositionPlacement(const Position& pos,
                                           const Resource& livestock) const {
  const auto& field = fields.at(pos);
  auto fieldType = field.getType();

  // 左下のマスの特別ルール
  if (pos == Position{2, 0}) {  // 左下の座標に応じて調整してください
    return livestock.getAmount() <= 1;  // 1匹まで
  }

  // 囲われていない厩の場合（STABLEフラグはあるがPASTUREフラグがない）
  if ((static_cast<int>(fieldType) & static_cast<int>(FieldType::STABLE)) &&
      !(static_cast<int>(fieldType) & static_cast<int>(FieldType::PASTURE))) {
    return livestock.getAmount() <= 1;  // 1匹まで
  }

  return false;  // その他の単独マスには配置不可
}

bool Farm::validateEnclosurePlacement(const size_t enclosureIndex,
                                      const Resource& livestock) const {
  // 牧場のインデックスの範囲チェック
  if (enclosureIndex >= enclosures.size()) {
    return false;
  }

  // 家畜かどうかの確認
  if (!(livestock.getType() == ResourceType::SHEEP ||
        livestock.getType() == ResourceType::BOAR ||
        livestock.getType() == ResourceType::CATTLE)) {
    return false;
  }

  // 収容可能数を超えていないか確認
  if (livestock.getAmount() > getMaxCapacity(enclosureIndex)) {
    return false;
  }

  return true;
}

bool Farm::placeLivestock(const std::vector<LivestockPlacement>& placements) {
  if (!validateLivestockPlacement(placements)) {
    return false;
  }

  // 家畜を配置
  for (const auto& placement : placements) {
    if (std::holds_alternative<size_t>(placement.location)) {
      // enclosureへの配置
      size_t enclosureIndex = std::get<size_t>(placement.location);
      const auto& enclosure = enclosures[enclosureIndex];
      int remainingLivestock = placement.livestock.getAmount();
      int baseAmount = remainingLivestock / enclosure.size();  // 基本の分配数
      int extraCount = remainingLivestock % enclosure.size();  // 余りの数

      for (const auto& pos : enclosure) {
        int amountForThisField = baseAmount;
        if (extraCount > 0) {
          // 余りを1つずつ追加で配置
          amountForThisField++;
          extraCount--;
        }

        if (amountForThisField > 0) {
          fields[pos].addContent(
              Resource(placement.livestock.getType(), amountForThisField));
        }
      }
    } else {
      // 特定マスへの配置（左下のマスまたは囲われていない厩）
      Position pos = std::get<Position>(placement.location);
      fields[pos].addContent(placement.livestock);
    }
  }
  return true;
}

bool Farm::canBuildRoom(Position pos, RoomType roomType) const {
  if (this->roomType != roomType) return false;
  if (!canPlace(pos)) return false;

  // 上下左右に隣接するマスをチェック
  bool hasAdjacentRoom = false;
  // 上のマス
  if (pos.x > 0) {
    Position newPos = pos.up();
    if (isRoom(fields.at(newPos).getType())) {
      hasAdjacentRoom = true;
    }
  }
  // 下のマス
  if (pos.x < 2) {
    Position newPos = pos.down();
    if (isRoom(fields.at(newPos).getType())) {
      hasAdjacentRoom = true;
    }
  }
  // 左のマス
  if (pos.y > 0) {
    Position newPos = pos.left();
    if (isRoom(fields.at(newPos).getType())) {
      hasAdjacentRoom = true;
    }
  }
  // 右のマス
  if (pos.y < 4) {
    Position newPos = pos.right();
    if (isRoom(fields.at(newPos).getType())) {
      hasAdjacentRoom = true;
    }
  }
  return hasAdjacentRoom;
}

bool Farm::buildRoom(Position pos, RoomType roomType) {
  if (!canBuildRoom(pos, roomType)) return false;

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

bool Farm::canPlowField(Position pos) const {
  if (!canPlace(pos)) return false;

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
  if (pos.x > 0) {
    Position newPos = pos.up();
    if (fields.at(newPos).getType() == FieldType::FIELD) {
      hasAdjacentField = true;
    }
  }
  // 下のマス
  if (pos.x < 2) {
    Position newPos = pos.down();
    if (fields.at(newPos).getType() == FieldType::FIELD) {
      hasAdjacentField = true;
    }
  }
  // 左のマス
  if (pos.y > 0) {
    Position newPos = pos.left();
    if (fields.at(newPos).getType() == FieldType::FIELD) {
      hasAdjacentField = true;
    }
  }
  // 右のマス
  if (pos.y < 4) {
    Position newPos = pos.right();
    if (fields.at(newPos).getType() == FieldType::FIELD) {
      hasAdjacentField = true;
    }
  }
  return hasAdjacentField;
}

bool Farm::plowField(Position pos) {
  if (!canPlowField(pos)) return false;
  fields[pos].plow();
  return true;
}
