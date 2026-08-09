namespace n2p{
    enum ShapeType {
        Circle,
        Rect,
        Polygon
    };

    class Shape {
    private:
        ShapeType type;
    public:
        virtual ~Shape() = default;

        virtual ShapeType GetType() const = 0;
    };
}