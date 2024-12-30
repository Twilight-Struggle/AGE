#include "resource.hpp"
#include <vector>

enum class FieldType {
    EMPTY,
    WOOD_ROOM,
    CLAY_ROOM,
    STONE_ROOM,
    FIELD,
    PASTURE,
    STABLE,
    OTHER,
};

class Field {
private:
    FieldType type;
    std::vector<Resource> contents; // 家畜や作物

public:
    Field();
    FieldType getType() const;
    void plow();
    void fence();
    void buildWoodRoom();
    void buildClayRoom();
    void buildStoneRoom();
    bool addContent(const Resource& resource);
    bool removeContent(const Resource& resource);
};

inline bool isRoom(FieldType type) {
    return type == FieldType::WOOD_ROOM ||
           type == FieldType::CLAY_ROOM ||
           type == FieldType::STONE_ROOM;
}