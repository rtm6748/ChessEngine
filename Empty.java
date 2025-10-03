import java.util.ArrayList;

public class Empty extends Piece {

    public Empty(Square square) {
        super(square, null);
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        return new ArrayList<>();
    }
}
