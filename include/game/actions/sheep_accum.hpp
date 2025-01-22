#pragma once
#include "game/actions/base_action.hpp"
#include "game/resource.hpp"

class SheepAccum : public BaseAction {
 private:
  Resource accumulatedResource;

 public:
  SheepAccum()
      : BaseAction(ActionType::SHEEP_ACCUM),
        accumulatedResource(ResourceType::SHEEP, 0) {}
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
