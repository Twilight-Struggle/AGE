#include "resource.hpp"
#include <vector>

enum class FieldType {
    EMPTY = 0,
    WOOD_ROOM = 1,
    CLAY_ROOM = 2,
    STONE_ROOM = 3,
    FIELD = 4,
    OTHER = 5,
    PASTURE = 1 << 4,    // 16
    STABLE = 1 << 5,     // 32
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
    void stable();
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