#include "field.hpp"
#include <vector>

enum class RoomType {
    WOOD,
    CLAY,
    STONE,
};

class Farm {
public:
    struct FencePosition {
        int x, y;    // マスの位置
        enum Edge {
            TOP,
            RIGHT,
            BOTTOM,
            LEFT
        } edge;       // マスのどの辺に位置するか
    };

private:
    std::vector<std::vector<Field>> fields;  // 3x5のグリッド
    std::vector<FencePosition> fences;       // 建てられた柵の位置
    
    int numRooms;
    int numStables;
    RoomType roomType;

    bool canPlace(int x, int y) const;
    bool canBuildRoom(int x, int y, RoomType roomType) const;
    bool canBuildFence(const std::vector<FencePosition>& newFences) const;
    bool canPlowField(int x, int y) const;
    bool isFenceAt(int x, int y, FencePosition::Edge edge) const;
    std::vector<std::vector<bool>> getEnclosedAreas(const std::vector<FencePosition>& tempFences) const;
    bool isValidEnclosure(const std::vector<std::vector<bool>>& newEnclosure) const;

public:
    Farm();
    bool buildRoom(int x, int y, RoomType roomType);
    bool buildFence(const std::vector<FencePosition>& newfences);
    bool plowField(int x, int y);
    // ...
};