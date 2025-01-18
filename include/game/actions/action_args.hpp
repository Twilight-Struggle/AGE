#pragma once
#include <variant>

#include "game/farm.hpp"

struct DayLabourArgs {};

struct PlowFieldArgs {
  Position position;
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