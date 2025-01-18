#include "game/action_space.hpp"

#include <algorithm>

ActionSpace::ActionSpace(std::unique_ptr<BaseAction> action)
    : action(std::move(action)) {
  occupiedPlayerIds.reserve(5);
}

bool ActionSpace::isOccupied() const { return !occupiedPlayerIds.empty(); }

bool ActionSpace::isOccupiedByPlayer(int playerId) const {
  return std::find(occupiedPlayerIds.begin(), occupiedPlayerIds.end(),
                   playerId) != occupiedPlayerIds.end();
}

bool ActionSpace::execute(Player& player, const ActionArgs& args) {
  if (isOccupied()) {
    return false;
  }

  bool result = action->execute(player, args);
  if (result) {
    occupiedPlayerIds.push_back(player.getPlayerId());
  }
  return result;
}

void ActionSpace::reset() { occupiedPlayerIds.clear(); }