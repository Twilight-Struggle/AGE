#include "game/player.hpp"

Player::Player(int id)
    : playerId(id), farm(), familyMembers(2), begginCards(0), resources() {
  // すべてのリソースタイプを0で初期化
  for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
    resources[i] = Resource(static_cast<ResourceType>(i), 0);
  }

  // 初期食料の設定
  resources[static_cast<size_t>(ResourceType::FOOD)] =
      Resource(ResourceType::FOOD, 3);
}

void Player::addResource(const Resource& resource) {
  resources[static_cast<size_t>(resource.getType())] += resource;
}

bool Player::useResource(const Resource& resource) {
  auto& playerResource = resources[static_cast<size_t>(resource.getType())];
  if (playerResource.getAmount() >= resource.getAmount()) {
    playerResource -= resource;
    return true;
  }
  return false;
}

bool Player::hasResource(const Resource& resource) const {
  return resources[static_cast<size_t>(resource.getType())].getAmount() >=
         resource.getAmount();
}

const Resource& Player::getResource(ResourceType type) const {
  return resources[static_cast<size_t>(type)];
}

bool Player::addFamilyMember() {
  if (familyMembers < 5) {
    familyMembers++;
    return true;
  }
  return false;
}

int Player::getFamilyMemberCount() const { return familyMembers; }

bool Player::feedFamily(const Resource& foodToUse) {
  if (foodToUse.getType() != ResourceType::FOOD) {
    return false;
  }

  int requiredFood = familyMembers * 2;
  auto& foodResource = resources[static_cast<size_t>(ResourceType::FOOD)];

  // 使用する食料が現在の所持量を超えていないかチェック
  if (foodToUse.getAmount() > foodResource.getAmount()) {
    return false;
  }

  // 指定された食料を消費
  foodResource -= foodToUse;

  // 食料が足りない場合、不足分の数だけ乞食カードを追加
  if (foodToUse.getAmount() < requiredFood) {
    int foodShortage = requiredFood - foodToUse.getAmount();
    begginCards += foodShortage;
    return true;
  }

  return true;
}

Farm& Player::getFarm() { return farm; }

const Farm& Player::getFarm() const { return farm; }

const int Player::getBegginCards() const { return begginCards; }