#include "game/resource.hpp"

Resource::Resource(ResourceType type, int amount)
    : type(type), amount(amount) {}

ResourceType Resource::getType() const { return type; }

int Resource::getAmount() const { return this->amount; }

void Resource::add(int amount) { this->amount += amount; }

void Resource::zero() { this->amount = 0; }

bool Resource::subtract(int amount) {
  if (this->amount >= amount) {
    this->amount -= amount;
    return true;
  }
  return false;
}

Resource& Resource::operator+=(const Resource& other) {
  if (this->type == other.type) {
    this->amount += other.amount;
  }
  return *this;
}

Resource& Resource::operator-=(const Resource& other) {
  if (this->type == other.type) {
    this->amount -= other.amount;
  }
  return *this;
}

Resource Resource::operator+(const Resource& other) const {
  Resource result(*this);
  result += other;
  return result;
}

Resource Resource::operator-(const Resource& other) const {
  Resource result(*this);
  result -= other;
  return result;
}

bool Resource::operator==(const Resource& other) const {
  return (this->type == other.type && this->amount == other.amount);
}
