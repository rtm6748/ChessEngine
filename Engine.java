import java.util.ArrayList;
import java.util.HashMap;

public class Engine {
    private ChessGame chessGame;

    public Engine(ChessGame chessGame) {
        this.chessGame = chessGame;
    }

    public PositionValue evaluatePosition(ChessGame chessGame) {
        HashMap<Piece, ArrayList<Square>> moves = chessGame.getGameBoard().getValidMoves(chessGame.getCurrentColor());
        HashMap<Piece, ArrayList<Square>> otherMoves = chessGame.getGameBoard().getValidMoves(chessGame.getCurrentColor() == Color.WHITE ? Color.BLACK : Color.WHITE);
        double currentColorValue = 0;
        double otherColorValue = 0;
        for (int i = 0; i < GameBoard.boardSize; ++i) {
            for (int j = 0; j < GameBoard.boardSize; ++j) {
                Piece piece = chessGame.getGameBoard().getPiece(new Square(i, j));
                if (piece.getClass() == Empty.class) {
                    continue;
                }
                if (piece.getColor() == chessGame.getCurrentColor()) {
                    currentColorValue += piece.getValue(chessGame.getGameBoard(), moves.get(piece));
                }
                else {
                    otherColorValue += piece.getValue(chessGame.getGameBoard(), otherMoves.get(piece));
                }
            }
        }
        return new PositionValue(currentColorValue, otherColorValue);
    }


    public Move findNextMove() {
        return findNextMove(this.chessGame, null, 5, false, false);
    }
    public Move findNextMove(ChessGame chessGame, Move firstMove, int depth, boolean low1, boolean low2) {
        HashMap<Piece, ArrayList<Square>> moves = this.chessGame.getGameBoard().getValidMoves(this.chessGame.getCurrentColor());
        PositionValue positionValue = evaluatePosition(chessGame);
        for (Piece piece : moves.keySet()) {
            for (Square square : moves.get(piece)) {
                if (firstMove == null) {
                    firstMove = new Move(piece, square);
                }
                ChessGame currGame = chessGame.getCopy();
                currGame.move(piece, square);
                PositionValue thisPosition = evaluatePosition(currGame);
                if (thisPosition.getDifference() < positionValue.getDifference()) {
                    if (low1) {
                        if (low2) {
                            return null;
                        }
                        return findNextMove(currGame, firstMove, depth - 1, true, true);
                    }
                    return findNextMove(currGame, firstMove, depth - 1, true, false);
                }
                return findNextMove(currGame, firstMove, depth - 1, false, false);
            }
        }
    }
}
