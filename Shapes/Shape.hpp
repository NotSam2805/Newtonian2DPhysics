namespace n2p{
    enum ShapeType {
        circle,
        rect,
        polygon
    };

    class Shape {
        private:
            ShapeType type;
        public:
            virtual ~Shape() = default;

            const virtual ShapeType GetType() const = 0;
    };
}