#include "Field.hpp"
#include <vector>

enum class RoomType {
    WOOD,
    CLAY,
    STONE,
};

class Farm {
private:
    std::vector<std::vector<Field>> fields; // 3x5のグリッド
    int numRooms;
    int numStables;
    RoomType roomType;

    bool canPlowField(int x, int y) const;

public:
    Farm();
    bool canBuildRoom(int x, int y, RoomType roomType) const;
    bool canBuildFence(int x, int y) const;
    bool canPlace(int x, int y) const;
    void buildRoom(int x, int y, RoomType roomType);
    void buildFence(int x, int y);
    void plowField(int x, int y);
    // ...
};