import java.util.ArrayList;

public class Pawn extends Piece{
    public Pawn(Square square, Color color) {
        super(square, color);
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        return null;
    }
}
