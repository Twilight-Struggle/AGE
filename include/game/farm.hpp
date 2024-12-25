#include "field.hpp"
#include <vector>

enum class RoomType {
    WOOD,
    CLAY,
    STONE,
};

class Farm {
private:
    struct FencePosition {
        int x, y;     // マスの位置
        enum Edge {
            TOP,
            RIGHT,
            BOTTOM,
            LEFT
        } edge;       // マスのどの辺に位置するか
    };

    std::vector<std::vector<Field>> fields;  // 3x5のグリッド
    std::vector<FencePosition> fences;       // 建てられた柵の位置
    
    int numRooms;
    int numStables;
    RoomType roomType;

    bool canPlace(int x, int y) const;
    bool canBuildRoom(int x, int y, RoomType roomType) const;
    bool canBuildFence(const std::vector<FencePosition>& newfences) const;
    bool canPlowField(int x, int y) const;
    bool isFenceAt(int x, int y, FencePosition::Edge edge) const;
    bool wouldCreateOrDivideEnclosure(int x, int y, FencePosition::Edge edge) const;
    std::vector<std::vector<bool>> getEnclosedAreas() const;
    bool isValidEnclosure(const std::vector<std::vector<bool>>& newEnclosure) const;

public:
    Farm();
    void buildRoom(int x, int y, RoomType roomType);
    void buildFence(const std::vector<FencePosition>& newfences);
    void plowField(int x, int y);
    // ...
};