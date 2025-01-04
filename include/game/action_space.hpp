#pragma once
#include "actions/base_action.hpp"
#include <memory>


class ActionSpace {
private:
    std::vector<int> occupiedPlayerIds;
    std::unique_ptr<BaseAction> action;

public:
    ActionSpace(std::unique_ptr<BaseAction> action);
    bool isOccupied() const;
    bool isOccupiedByPlayer(int playerId) const;
    bool execute(Player& player);
    void reset();
    
    void roundStart() { action->roundStart(); }
}; 