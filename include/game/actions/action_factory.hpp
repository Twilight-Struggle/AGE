#pragma once
#include "game/actions/base_action.hpp"
#include <vector>
#include <memory>

class ActionFactory {
public:
    static std::vector<std::unique_ptr<BaseAction>> createAllActions() {
        std::vector<std::unique_ptr<BaseAction>> actions;
        
        // 資源蓄積アクション
        // actions.push_back(std::make_unique<WoodAccumulation>());
        // actions.push_back(std::make_unique<ClayAccumulation>());
        // actions.push_back(std::make_unique<ReedAccumulation>());

        // 農業アクション
        // actions.push_back(std::make_unique<Sow>());
        // actions.push_back(std::make_unique<PlowField>());
        // ...

        return actions;
    }
}; 