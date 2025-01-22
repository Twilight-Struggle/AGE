#pragma once
#include "game/actions/base_action.hpp"

class BuildFences : public BaseAction {
 public:
  BuildFences() : BaseAction(ActionType::BUILD_FENCES) {}
  bool execute(Player& player, const ActionArgs& args) override {
    if (const auto* fenceArgs = std::get_if<BuildFenceArgs>(&args)) {
      // Positionを使用して畑を耕す
      if (player.getFarm().buildFence(fenceArgs->fencePositions)) {
        return true;
      }
    }
    return false;
  }

  void roundStart() override {}
};
