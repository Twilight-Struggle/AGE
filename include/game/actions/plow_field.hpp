#pragma once
#include "game/actions/base_action.hpp"

class PlowField : public BaseAction {
 public:
  PlowField() : BaseAction(ActionType::PLOW_FIELD) {}
  bool execute(Player& player, const ActionArgs& args) override {
    if (auto* plowArgs = std::get_if<PlowFieldArgs>(&args)) {
      // Positionを使用して畑を耕す
      if (player.getFarm().plowField(plowArgs->positions)) {
        return true;
      }
    }
    return false;
  }

  void roundStart() override {}
};
