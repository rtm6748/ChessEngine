import java.util.ArrayList;
import java.util.HashMap;

public class ChessGame {
    private GameBoard gameBoard;
    private Color currentColor;

    public ChessGame getCopy() {
        return new ChessGame(gameBoard, currentColor);
    }


    public Color getCurrentColor() {
        return currentColor;
    }

    public GameBoard getGameBoard() {
        return gameBoard;
    }

    public ChessGame() {
        gameBoard = new GameBoard();
        currentColor = Color.WHITE;
    }

    public ChessGame(GameBoard gameboard, Color currentColor) {
        this.gameBoard = gameboard;
        this.currentColor = currentColor;
    }

    public Piece getPiece(Square square) {
        return gameBoard.getPiece(square);
    }

    public boolean move(Piece piece, Square square) {
        if (piece.getColor() != currentColor) {
            return false;
        }
        HashMap<Piece, ArrayList<Square>> validMoves = gameBoard.getValidMoves(currentColor);
        if (validMoves.get(piece) == null) {
            return false;
        }
        if (validMoves.get(piece).contains(square)) {
            gameBoard.move(piece, square);
            currentColor = currentColor == Color.WHITE ? Color.BLACK : Color.WHITE;
            return true;
        }
        return false;
    }
}
