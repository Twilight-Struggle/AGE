#pragma once
#include "game/actions/base_action.hpp"
#include "game/resource.hpp"

class Fishing : public BaseAction {
 private:
  Resource accumulatedResource;

 public:
  Fishing()
      : BaseAction(ActionType::FISHING),
        accumulatedResource(ResourceType::FOOD, 0) {}
  bool execute(Player& player, const ActionArgs& args) override {
    if (accumulatedResource.getAmount() <= 0) {
      return false;
    }
    player.addResource(accumulatedResource);
    accumulatedResource.zero();
    return true;
  }

  void roundStart() override { accumulatedResource.add(1); }
};
