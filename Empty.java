import java.util.ArrayList;

public class Empty extends Piece {

    public Empty(Square square, Color color) {
        super(square, color);
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        return new ArrayList<>();
    }


    @Override
    public String toString() {
        return this.getColor() == Color.WHITE ? "■" : "□";
    }
}
