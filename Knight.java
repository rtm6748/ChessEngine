import java.util.ArrayList;

public class Knight extends Piece {
    public Knight(Square square, Color color) {
        super(square, color);
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        int[] MovesX = {-2, -1, 1, 2};
        int[] MovesY = {-2, -1, 1, 2};
        ArrayList<Square> validMoves = new ArrayList<>();
        for (int i = 0; i < MovesX.length; ++i) {
            for (int j = 0; j < MovesY.length; ++j) {
                if (i == j) {
                    continue;
                }
                Square newSquare = new Square(square, MovesX[i], MovesY[j]);
                if (isValid(newSquare) && currBoard.getPiece(newSquare).getColor() != this.getColor()) {
                    validMoves.add(newSquare);
                }
            }
        }
        return validMoves;
    }
}
