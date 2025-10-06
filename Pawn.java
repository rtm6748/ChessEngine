import java.util.ArrayList;

public class Pawn extends Piece{
    public Pawn(Square square, Color color) {
        super(square, color);
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        ArrayList<Square> validMoves = new ArrayList<>();
        int forwardOrBackward = -1;
        if (this.getColor() == Color.WHITE) {
            forwardOrBackward = 1;
        }
        Square forward1 = new Square(this.getCurrSquare(), 0, forwardOrBackward);
        if (isValid(forward1) && currBoard.getPiece(forward1).getClass() == Empty.class) {
            validMoves.add(forward1);
            Square forward2 = new Square(this.getCurrSquare(), 0, 2 * forwardOrBackward);
            boolean onStart = this.getCurrSquare().getY() == 1 && this.getColor() == Color.WHITE ||
                              this.getCurrSquare().getY() == 7 && this.getColor() == Color.BLACK;
            if (isValid(forward1) && currBoard.getPiece(forward2).getClass() == Empty.class && onStart) {
                validMoves.add(forward2);
            }
        }
        Square left = new Square(this.getCurrSquare(), -1, forwardOrBackward);
        Square right = new Square(this.getCurrSquare(), 1, forwardOrBackward);
        Color other = this.getColor() == Color.WHITE ? Color.BLACK : Color.WHITE;
        if (isValid(left) && currBoard.getPiece(left).getColor() == other) {
            validMoves.add(left);
        }
        if (isValid(right) && currBoard.getPiece(right).getColor() == other) {
            validMoves.add(right);
        }
        return validMoves;
    }


    @Override
    public String toString() {
        return getColor() == Color.WHITE ? "♟" : "♙";
    }
}
