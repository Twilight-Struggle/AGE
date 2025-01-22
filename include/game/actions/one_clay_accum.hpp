#pragma once
#include "game/actions/base_action.hpp"
#include "game/resource.hpp"

class OneClayAccum : public BaseAction {
 private:
  Resource accumulatedResource;

 public:
  OneClayAccum()
      : BaseAction(ActionType::ONE_CLAY_ACCUM),
        accumulatedResource(ResourceType::CLAY, 0) {}
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
