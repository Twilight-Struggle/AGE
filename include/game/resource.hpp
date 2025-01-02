enum class ResourceType {
    WOOD,
    CLAY,
    REED,
    STONE,
    GRAIN,
    VEGETABLE,
    SHEEP,
    BOAR,
    CATTLE,
    FOOD,
    OTHER,
    COUNT,
};

class Resource {
private:
    ResourceType type;
    int amount;

public:
    Resource() : type(ResourceType::FOOD), amount(0) {}
    Resource(ResourceType type, int amount);
    ResourceType getType() const;
    int getAmount() const;
    void add(int amount);
    bool subtract(int amount);
    
    // 演算子のオーバーロード
    Resource& operator+=(const Resource& other);
    Resource& operator-=(const Resource& other);
    Resource operator+(const Resource& other) const;
    Resource operator-(const Resource& other) const;
    bool operator==(const Resource& other) const;
}; 