#include "game/Field.hpp"

Field::Field() : type(FieldType::EMPTY), contents() {}

FieldType Field::getType() const {
    return type;
}

// このクラスでは建てられるかどうかの判定は行わない
void Field::plow() {
    type = FieldType::FIELD;
}

void Field::fence() {
    type = FieldType::PASTURE;
}

void Field::buildWoodRoom() {
    type = FieldType::WOOD_ROOM;
}

void Field::buildClayRoom() {
    type = FieldType::CLAY_ROOM;
}

void Field::buildStoneRoom() {
    type = FieldType::STONE_ROOM;
}

bool Field::addContent(const Resource& resource) {
    // Check if resource type is valid for the field type
    bool isLivestock = (resource.getType() == ResourceType::SHEEP ||
                       resource.getType() == ResourceType::BOAR ||
                       resource.getType() == ResourceType::CATTLE);
    bool isCrop = (resource.getType() == ResourceType::GRAIN ||
                  resource.getType() == ResourceType::VEGETABLE);
                  
    if ((isLivestock && type != FieldType::PASTURE) ||
        (isCrop && type != FieldType::FIELD) ||
        (!isLivestock && !isCrop)) {
        return false;
    }

    // 既存のリソースがある場合は加算
    if (!contents.empty()) {
        if (contents[0].getType() == resource.getType()) {
            contents[0] += resource;
            return true;
        }
        return false;
    } else {
        // 新しいリソースとして追加
        contents.push_back(resource);
        return true;
    }

}

bool Field::removeContent(const Resource& resource) {
    for (auto& content : contents) {
        if (content.getType() == resource.getType()) {
            if (content.subtract(resource.getAmount())) {
                if (content.getAmount() == 0) {
                    std::erase(contents, content);
                }
                return true;
            }
            return false;
        }
    }
    return false;
}