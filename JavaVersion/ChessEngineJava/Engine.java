package ChessEngineJava;

import java.util.ArrayList;
import java.util.HashMap;

public class Engine {
    private final ChessGame currentBoard;

    public Engine(ChessGame chessGame) {
        this.currentBoard = chessGame;
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
        return findNextMove(this.currentBoard, 3);
    }
    public Move findNextMove(ChessGame chessGame, int depth) {
        double bestValue = Double.NEGATIVE_INFINITY;
        Move bestMove = null;

        HashMap<Piece, ArrayList<Square>> moves = chessGame.getGameBoard().getValidMoves(chessGame.getCurrentColor());

        for (Piece piece : moves.keySet()) {
            for (Square square : moves.get(piece)) {
                ChessGame newGame = chessGame.getCopy();
                Piece copiedPiece = newGame.getGameBoard().getPiece(piece.getCurrSquare());
                newGame.move(copiedPiece, square);

                double value = minimax(newGame, depth - 1, false);  // opponent's turn

                if (value > bestValue) {
                    bestValue = value;
                    bestMove = new Move(piece, square);
                }
            }
        }

        return bestMove;
    }

    private double minimax(ChessGame game, int depth, boolean maximizingPlayer) {
        if (depth == 0) {
            return evaluatePosition(game).getDifference();
        }

        HashMap<Piece, ArrayList<Square>> moves = game.getGameBoard().getValidMoves(game.getCurrentColor());

        // Handle checkmate or stalemate
        if (moves.isEmpty()) {
            return evaluatePosition(game).getDifference();
        }

        double bestValue = maximizingPlayer ? Double.NEGATIVE_INFINITY : Double.POSITIVE_INFINITY;

        for (Piece piece : moves.keySet()) {
            for (Square square : moves.get(piece)) {
                ChessGame newGame = game.getCopy();
                Piece copiedPiece = newGame.getGameBoard().getPiece(piece.getCurrSquare());
                newGame.move(copiedPiece, square);

                double eval = minimax(newGame, depth - 1, !maximizingPlayer);

                if (maximizingPlayer) {
                    bestValue = Math.max(bestValue, eval);
                } else {
                    bestValue = Math.min(bestValue, eval);
                }
            }
        }

        return bestValue;
    }


    /*public ChessEngineJava.Move findNextMove() {
        return findNextMove(this.chessGame, null, 5, false, false);
    }
    public ChessEngineJava.Move findNextMove(ChessEngineJava.ChessGame chessGame, ChessEngineJava.Move firstMove, int depth, boolean low1, boolean low2) {
        HashMap<ChessEngineJava.Piece, ArrayList<ChessEngineJava.Square>> moves = this.chessGame.getGameBoard().getValidMoves(this.chessGame.getCurrentColor());
        ChessEngineJava.PositionValue positionValue = evaluatePosition(chessGame);
        Double highestVal = null;
        ChessEngineJava.Piece highestPiece = null;
        ChessEngineJava.Square highestSquare = null;
        for (ChessEngineJava.Piece piece : moves.keySet()) {
            for (ChessEngineJava.Square square : moves.get(piece)) {
                if (firstMove == null) {
                    firstMove = new ChessEngineJava.Move(piece, square);
                }
                ChessEngineJava.ChessGame currGame = chessGame.getCopy();
                currGame.move(piece, square);
                ChessEngineJava.PositionValue thisPosition = evaluatePosition(currGame);
                if (highestVal == null || thisPosition.getDifference() > highestVal) {
                    highestVal = thisPosition.getDifference();
                    highestPiece = piece;
                    highestSquare = square;
                }

                if (depth != 0) {
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
        return new ChessEngineJava.Move(highestPiece, highestSquare);
    }*/

}
