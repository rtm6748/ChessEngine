import java.util.ArrayList;

public class Queen extends Piece {
    public Queen(Square square, Color color) {
        super(square, color);
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        Piece currentPiece = currBoard.getPiece(square);
        ArrayList<Square> validMoves = new ArrayList<>();
        int[] dirX = {1, 1, 1, 0, 0, -1, -1, -1};
        int[] dirY = {1, 0, -1, 1, -1, 1, 0, -1};
        for (int i = 0; i < dirX.length; ++i) {
            for (int iteration = 1; iteration < GameBoard.boardSize; ++iteration) {
                Square newSquare = new Square(currentPiece.getCurrSquare(), dirX[i] * iteration, dirY[i] * iteration);
                Piece newPiece = currBoard.getPiece(newSquare);
                if (!isValid(newSquare)) {
                    break;
                }
                if (newPiece.getColor() == this.getColor()) {
                    break;
                }
                if (newPiece.getColor() != Color.NONE && newPiece.getColor() != this.getColor()) {
                    validMoves.add(newSquare);
                    break;
                }
                validMoves.add(newSquare);
            }

        }
        return validMoves;
    }

}
