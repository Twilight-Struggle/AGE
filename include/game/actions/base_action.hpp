#pragma once
#include "game/actions/action_args.hpp"
#include "game/player.hpp"

enum class ActionType {
  BUILD_ROOMS,
  START_PLAYER,
  GAIN_GRAIN,
  PLOW_FIELD,
  OCCUPATION,
  DAY_LABOUR,
  THREE_WOODS_ACCUM,
  ONE_CLAY_ACCUM,
  ONE_REED_ACCUM,
  FISHING,
  BUILD_MAJOR_IMPROVEMENT,
  SHEEP_ACCUM,
  BUILD_FENCES,
  SOW_AND_BREAD,
  GROWTH_FAMILY,
  RENNOVATION_AND_IMPROVEMENT,
  STONE_ACCUM,
  GAIN_VEGETABLE,
  BOAR_ACCUM,
  STONE_ACCUM2,
  CATTLE_ACCUM,
  GROWTH_FAMILY_WITHOUT_ROOM,
  FIELD_AND_SOW,
  RENNOVATION_AND_FENCE,
  FOUR_WOODS_ACCUM,
  THREE_CLAYS_ACCUM,
  REED_ACCUM_STONE_WOOD,
  ANIMAL_MARKET,
  OCCUPATION2,
  TRAVELING_PLAYER,
};

class BaseAction {
 private:
  ActionType actionType;

 public:
  BaseAction(ActionType actionType) : actionType(actionType) {}
  virtual ~BaseAction() = default;
  virtual bool execute(Player& player, const ActionArgs& args) = 0;
  virtual void roundStart() = 0;  // ラウンド開始時の処理
  ActionType getActionType() const { return actionType; }
};