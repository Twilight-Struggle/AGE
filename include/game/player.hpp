#pragma once
#include "farm.hpp"
#include <array>

class Player {
private:
    int playerId;
    Farm farm;
    std::array<Resource, static_cast<size_t>(ResourceType::COUNT)> resources;
    int familyMembers;
    int begginCards;

public:
    Player(int id);
    int getPlayerId() const { return playerId; }
    // リソース関連
    void addResource(const Resource& resource);
    bool useResource(const Resource& resource);
    bool hasResource(const Resource& resource) const;
    const Resource& getResource(ResourceType type) const;
    
    // 家族関連
    bool addFamilyMember();
    int getFamilyMemberCount() const;
    bool feedFamily(const Resource& foodToUse);
    
    // 農場関連
    Farm& getFarm();
    const Farm& getFarm() const;

    const int getBegginCards() const;
}; 