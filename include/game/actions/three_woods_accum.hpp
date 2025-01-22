#pragma once
#include "game/actions/base_action.hpp"
#include "game/resource.hpp"

class ThreeWoodsAccum : public BaseAction {
 private:
  Resource accumulatedResource;

 public:
  ThreeWoodsAccum()
      : BaseAction(ActionType::THREE_WOODS_ACCUM),
        accumulatedResource(ResourceType::WOOD, 0) {}
  bool execute(Player& player, const ActionArgs& args) override {
    if (accumulatedResource.getAmount() <= 0) {
      return false;
    }
    player.addResource(accumulatedResource);
    accumulatedResource.zero();
    return true;
  }

  void roundStart() override { accumulatedResource.add(3); }
};
