import java.util.ArrayList;

public class Pawn extends Piece{
    public Pawn(Square square, Color color) {
        super(square, color);
    }

    public double getValue(GameBoard gameBoard, ArrayList<Square> moves) {

        double val = 1;
        double valPerPieceDefended = 0.4;
        double valPerPieceAttacked = 0.3;
        double valDefendedByPawn = 0.3;
        int black = -1;
        if (this.getColor() == Color.WHITE) {
            black = 1;
        }

        int x = this.getCurrSquare().getX();
        int y = this.getCurrSquare().getY();

        if (x - 1 >= 0) {
            Piece piece = gameBoard.getPiece(new Square(x - 1, y + black));
            if (piece.getClass() != Empty.class) {
                if (piece.getColor() == this.getColor()) {
                    val += valPerPieceDefended;
                }
                else {
                    val += valPerPieceAttacked;
                }
            }
        }
        if (x + 1 < 8) {
            Piece piece = gameBoard.getPiece(new Square(x + 1, y + black));
            if (piece.getClass() != Empty.class) {
                if (piece.getColor() == this.getColor()) {
                    val += valPerPieceDefended;
                }
                else {
                    val += valPerPieceAttacked;
                }
            }
        }
        if (x - 1 >= 0) {
            Piece piece = gameBoard.getPiece(new Square(x - 1, y - black));
            if (piece.getClass() == Pawn.class) {
                if (piece.getColor() == this.getColor()) {
                    val += valDefendedByPawn;
                }
            }
        }
        if (x + 1 < 8) {
            Piece piece = gameBoard.getPiece(new Square(x + 1, y - black));
            if (piece.getClass() == Pawn.class) {
                if (piece.getColor() == this.getColor()) {
                    val += valPerPieceDefended;
                }
            }
        }

        int[] checkX = {-1, 1};
        int[] checkY = {1, 1};

        return val;
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
                              this.getCurrSquare().getY() == 6 && this.getColor() == Color.BLACK;
            if (isValid(forward1) && currBoard.getPiece(forward2).getClass() == Empty.class && onStart) {
                validMoves.add(forward2);
            }
        }
        Square left = new Square(this.getCurrSquare(), -1, forwardOrBackward);
        Square right = new Square(this.getCurrSquare(), 1, forwardOrBackward);
        Color other = this.getColor() == Color.WHITE ? Color.BLACK : Color.WHITE;
        if (isValid(left) && currBoard.getPiece(left).getColor() == other && currBoard.getPiece(left).getClass() != Empty.class) {
            validMoves.add(left);
        }
    if (isValid(right) && currBoard.getPiece(right).getColor() == other && currBoard.getPiece(right).getClass() != Empty.class) {
            validMoves.add(right);
        }
        return validMoves;
    }


    @Override
    public String toString() {
        return getColor() == Color.WHITE ? "WP" : "BP";
        //return getColor() == Color.WHITE ? "♟" : "♙";
    }
}
