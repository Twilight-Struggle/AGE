#pragma once
#include "game/actions/base_action.hpp"

class DayLabour : public BaseAction {
 public:
  DayLabour() : BaseAction(ActionType::DAY_LABOUR) {}
  bool execute(Player& player, const ActionArgs& args) override {
    player.addResource(Resource(ResourceType::FOOD, 2));
    return true;
  }

  void roundStart() override {}
};
