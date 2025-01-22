#pragma once
#include "game/actions/base_action.hpp"

class GainGrain : public BaseAction {
 public:
  GainGrain() : BaseAction(ActionType::GAIN_GRAIN) {}
  bool execute(Player& player, const ActionArgs& args) override {
    player.addResource(Resource(ResourceType::GRAIN, 1));
    return true;
  }

  void roundStart() override {}
};
