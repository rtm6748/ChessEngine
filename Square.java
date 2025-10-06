public class Square {
    private final int x;
    private final int y;

    public Square(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public Square(Square square, int offX, int offY) {
        this(square.getX() + offX, square.getY() + offY);
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }


    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Square square) {
            return this.getX() == square.getX() && this.getY() == square.getY();
        }
        return false;
    }

    @Override
    public String toString() {
        return "X: " + this.getX() + " Y: " + this.getY();
    }
}
