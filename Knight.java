import java.util.ArrayList;

public class Knight extends Piece {

    public Knight(Square square, Color color) {
        super(square, color);
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
        for (int i = 0; i < MovesX.length; ++i) {
            for (int j = 0; j < MovesY.length; ++j) {
                if (Math.abs(MovesX[i]) == Math.abs(MovesY[j])) {
                    continue;
                }
                Square newSquare = new Square(square, MovesX[i], MovesY[j]);
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
}
