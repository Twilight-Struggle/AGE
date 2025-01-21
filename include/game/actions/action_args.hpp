#pragma once
#include <variant>

#include "game/farm.hpp"

struct DayLabourArgs {};

struct PlowFieldArgs {
  std::set<Position> positions;
};

// struct BuildFenceArgs {
//   std::vector<FencePosition> fencePositions;
// };

// struct BuildRoomArgs {
//   Position position;
//   RoomType roomType;
// };

// 他のアクションに必要な引数も同様に定義
using ActionArgs = std::variant<DayLabourArgs, PlowFieldArgs>;