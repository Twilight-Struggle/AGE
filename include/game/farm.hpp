#pragma once
#include <map>
#include <optional>
#include <set>
#include <variant>
#include <vector>

#include "field.hpp"

enum class RoomType {
  WOOD,
  CLAY,
  STONE,
};

struct Position {
  static constexpr int WIDTH = 3;
  static constexpr int HEIGHT = 5;
  int x;
  int y;

  Position(int x, int y) : x(x), y(y) {}

  Position up() const { return Position(x - 1, y); }

  Position down() const { return Position(x + 1, y); }

  Position left() const { return Position(x, y - 1); }

  Position right() const { return Position(x, y + 1); }

  // 比較演算子（setで使用するため必要）
  bool operator<(const Position& other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
  }

  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }
};

// 新しい入力形式を表現する型
struct LivestockPlacement {
  std::variant<size_t, Position>
      location;  // enclosureのindexまたは特定のPosition
  Resource livestock;
};

class FencePosition {
 public:
  enum Edge {
    TOP,
    LEFT,
    BOTTOM,  // x=2の場合のみ
    RIGHT    // y=4の場合のみ
  };
  Position getPosition() const { return pos; }
  Edge getEdge() const { return edge; }

  // コンストラクタでバリデーション
  static std::optional<FencePosition> create(const Position& pos,
                                             const Edge& edge) {
    FencePosition fencePos(pos, edge);
    if (fencePos.isValid()) {
      return fencePos;
    }
    return std::nullopt;
  }

  // std::setで使用するための比較演算子
  bool operator<(const FencePosition& other) const {
    if (pos.x != other.pos.x) return pos.x < other.pos.x;
    if (pos.y != other.pos.y) return pos.y < other.pos.y;
    return edge < other.edge;
  }

  bool operator==(const FencePosition& other) const {
    return pos.x == other.pos.x && pos.y == other.pos.y && edge == other.edge;
  }

 private:
  FencePosition(const Position& pos, const Edge& edge) : pos(pos), edge(edge) {}
  Position pos;
  Edge edge;
  // 有効な柵の位置かチェック
  bool isValid() const {
    if (pos.x < 0 || pos.x >= 3 || pos.y < 0 || pos.y >= 5) return false;

    switch (edge) {
      case TOP:
      case LEFT:
        return true;  // 常に有効
      case BOTTOM:
        return pos.x == 2;  // 最下段のみ
      case RIGHT:
        return pos.y == 4;  // 最右列のみ
      default:
        return false;
    }
  }
};

class Farm {
 private:
  std::map<Position, Field> fields;            // 3x5のグリッド
  std::vector<FencePosition> fences;           // 建てられた柵の位置
  std::vector<std::set<Position>> enclosures;  // 牧場

  int numRooms;
  int numStables;
  RoomType roomType;

  bool canPlace(const Position& pos) const;
  bool isConnected(const std::set<Position>& positions) const;
  bool canBuildRoom(const std::set<Position>& pos,
                    const RoomType roomType) const;
  std::pair<bool, std::vector<std::set<Position>>> canBuildFence(
      const std::vector<FencePosition>& newFences) const;
  bool canPlowField(const std::set<Position>& positions) const;
  bool isFenceAt(const Position& pos, const FencePosition::Edge& edge) const;
  std::pair<bool, std::set<Position>> isEnclosed(
      const Position& start, const std::set<FencePosition>& fences) const;
  std::vector<std::set<Position>> getEnclosedAreas(
      const std::set<FencePosition>& fences) const;
  std::pair<bool, std::vector<std::set<Position>>> isValidEnclosure(
      const std::vector<std::set<Position>>& newEnclosure,
      const std::set<FencePosition>& tempFences) const;
  bool canBuildStable(const Position& pos) const;
  int getStableCount(const std::set<Position>& enclosure) const;
  int getMaxCapacity(const size_t enclosureIndex) const;
  bool validateLivestockPlacement(
      const std::vector<LivestockPlacement>& placements) const;
  bool validateSinglePositionPlacement(const Position& pos,
                                       const Resource& livestock) const;
  bool validateEnclosurePlacement(const size_t enclosureIndex,
                                  const Resource& livestock) const;

 public:
  Farm();
  bool buildRoom(const std::set<Position>& pos, RoomType roomType);
  bool buildFence(const std::vector<FencePosition>& newfences);
  bool plowField(const std::set<Position>& positions);
  bool buildStable(const Position& pos);
  bool placeLivestock(const std::vector<LivestockPlacement>& placements);
  // const参照を返すgetter
  const Field& getField(const Position& pos) const { return fields.at(pos); }
  // イテレーションが必要な場合
  const std::map<Position, Field>& getFields() const { return fields; }
};