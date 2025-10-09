import java.util.ArrayList;
import java.util.Objects;

public class Knight extends Piece {

    public Knight(Square square, Color color) {
        super(square, color);
    }

    public Knight getCopy() {
        return new Knight(new Square(this.getCurrSquare().getX(), this.getCurrSquare().getY()), this.getColor());
    }

    public double getValue(GameBoard gameBoard, ArrayList<Square> moves) {
        double val = 3.0;
        double distanceForwardMult = 0.3;
        double centeredMult = 0.2;
        Color color = this.getColor();
        if (color == Color.BLACK) {
            int x = this.getCurrSquare().getX();
            int y = this.getCurrSquare().getY();
            val += (7 - y) * distanceForwardMult;
            val += Math.abs(3.5 - x) * centeredMult;
            val += Math.abs(3.5 - y) * centeredMult;
        }
        return val;
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        int[] MovesX = {-2, -1, 1, 2};
        int[] MovesY = {-2, -1, 1, 2};
        ArrayList<Square> validMoves = new ArrayList<>();
        for (int movesX : MovesX) {
            for (int i : MovesY) {
                if (Math.abs(movesX) == Math.abs(i)) {
                    continue;
                }
                Square newSquare = new Square(square, movesX, i);
                if (!isValid(newSquare)) {
                    continue;
                }
                if (currBoard.getPiece(newSquare).getClass() != Empty.class) {
                    if (currBoard.getPiece(newSquare).getColor() == this.getColor()) {
                        continue;
                    }
                }
                validMoves.add(newSquare);
            }
        }
        return validMoves;
    }


    @Override
    public String toString() {
        return getColor() == Color.WHITE ? "WN" : "BN";
        //return getColor() == Color.WHITE ? "♞" : "♘";
    }

   /* @Override
    public boolean equals(Object obj) {
        if (obj instanceof Knight knight) {
            return this.getCurrSquare().equals(knight.getCurrSquare()) && this.getColor() == knight.getColor();
        }
        return false;
    }

    @Override
    public int hashCode() {
        Square s = this.getCurrSquare();
        return Objects.hash(this.getColor(), this.getClass()) + s.hashCode();
    }*/

    @Override
    public boolean equals(Object o) {
        return this == o;
    }

    @Override
    public int hashCode() {
        return System.identityHashCode(this);
    }


}
