package ChessEngineJava;

import java.util.ArrayList;

public class Queen extends Piece {
    public Queen(Square square, Color color) {
        super(square, color);
    }


    public double getValue(GameBoard gameBoard, ArrayList<Square> moves) {
        double val = 9;
        double valPerSquareMulti = 0.1;
        double valPerOtherColor = 1;
        if (moves != null) {
            for (Square move : moves) {
                if (gameBoard.getPiece(move).getColor() != this.getColor()) {
                    val += valPerOtherColor;
                }
            }
            val += moves.size() * valPerSquareMulti;
        }
        return val;
    }

    public Queen getCopy() {
        return new Queen(new Square(this.getCurrSquare().getX(), this.getCurrSquare().getY()), this.getColor());
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
                if (!isValid(newSquare)) {
                    break;
                }
                Piece newPiece = currBoard.getPiece(newSquare);
                if (newPiece.getClass() != Empty.class) {
                    if (newPiece.getColor() == this.getColor()) {
                        break;
                    }
                    if (newPiece.getColor() != this.getColor()) {
                        validMoves.add(newSquare);
                        break;
                    }
                }
                validMoves.add(newSquare);
            }

        }
        return validMoves;
    }


    @Override
    public String toString() {
        return getColor() == Color.WHITE ? "WQ" : "BQ";
        //return getColor() == ChessEngineJava.Color.WHITE ? "♛" : "♕";
    }

    /*@Override
    public boolean equals(Object obj) {
        if (obj instanceof ChessEngineJava.Queen queen) {
            return this.getCurrSquare().equals(queen.getCurrSquare()) && this.getColor() == queen.getColor();
        }
        return false;
    }

    @Override
    public int hashCode() {
        ChessEngineJava.Square s = this.getCurrSquare();
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
