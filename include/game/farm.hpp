#include "field.hpp"
#include <vector>
#include <stdexcept>
#include <optional>
#include <set>

enum class RoomType {
    WOOD,
    CLAY,
    STONE,
};

struct Position {
    int x;
    int y;
    
    Position(int x, int y) : x(x), y(y) {}
    
    // 比較演算子（setで使用するため必要）
    bool operator<(const Position& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class FencePosition {
public:
    enum Edge {
        TOP,
        LEFT,
        BOTTOM,  // x=2の場合のみ
        RIGHT    // y=4の場合のみ
    };
    int getX() const { return x; }
    int getY() const { return y; }
    Edge getEdge() const { return edge; }

    // コンストラクタでバリデーション
    static std::optional<FencePosition> create(int x, int y, Edge edge) {
        FencePosition pos(x, y, edge);
        if (pos.isValid()) {
            return pos;
        }
        return std::nullopt;
    }

    // std::setで使用するための比較演算子
    bool operator<(const FencePosition& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return edge < other.edge;
    }

    bool operator==(const FencePosition& other) const {
        return x == other.x && y == other.y && edge == other.edge;
    }

private:
    FencePosition(int x, int y, Edge edge) : x(x), y(y), edge(edge) {}
    int x, y;    // マスの位置
    Edge edge;
    // 有効な柵の位置かチェック
    bool isValid() const {
        if (x < 0 || x >= 3 || y < 0 || y >= 5) return false;
        
        switch (edge) {
            case TOP:
            case LEFT:
                return true;  // 常に有効
            case BOTTOM:
                return x == 2;  // 最下段のみ
            case RIGHT:
                return y == 4;  // 最右列のみ
            default:
                return false;
        }
    }
};

class Farm {
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
    std::pair<bool, std::set<Position>> isEnclosed(const Position& start, const std::set<FencePosition>& fences) const;
    std::vector<std::set<Position>>  getEnclosedAreas(const std::set<FencePosition>& fences) const;
    bool isValidEnclosure(const std::vector<std::set<Position>>& newEnclosure, 
                           const std::set<FencePosition>& tempFences) const;

public:
    Farm();
    bool buildRoom(int x, int y, RoomType roomType);
    bool buildFence(const std::vector<FencePosition>& newfences);
    bool plowField(int x, int y);
    // ...
};