import java.util.ArrayList;
import java.util.HashMap;

public class ChessGame {
    private GameBoard gameBoard;
    private Color currentColor;

    public ChessGame() {
        gameBoard = new GameBoard();
        currentColor = Color.WHITE;
    }

    public boolean move(Piece piece, Square square) {
        if (piece.getColor() != currentColor) {
            return false;
        }
        HashMap<Piece, ArrayList<Square>> validMoves = gameBoard.getValidMoves(Color.WHITE);
        if (validMoves.get(piece).contains(square)) {
            move(piece, square);
            return true;
        }
        return false;
    }
}
