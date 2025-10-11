package ChessEngineJava;

import java.util.ArrayList;

public class Bishop extends Piece {
    public Bishop(Square square, Color color) {
        super(square, color);
    }

    public double getValue(GameBoard gameBoard, ArrayList<Square> moves) {
        double val = 3.2;
        double valPerSquareMult = 0.1;
        double valPerOtherColor = 1;
        if (moves != null) {
            for (Square move : moves) {
                if (gameBoard.getPiece(move).getColor() != this.getColor()) {
                    val += valPerOtherColor;
                }
            }
            val += moves.size() * valPerSquareMult;
        }
        return val;
    }

    public Bishop getCopy() {
        return new Bishop(new Square(this.getCurrSquare().getX(), this.getCurrSquare().getY()), this.getColor());
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        Piece currentPiece = currBoard.getPiece(square);
        ArrayList<Square> validMoves = new ArrayList<>();
        int[] dirX = {1, 1, -1, -1};
        int[] dirY = {1, -1, 1, -1};
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
        return getColor() == Color.WHITE ? "WB" : "BB";
        //return getColor() == ChessEngineJava.Color.WHITE ? "♝" : "♗";
    }

   /* @Override
    public boolean equals(Object obj) {
        if (obj instanceof ChessEngineJava.Bishop bishop) {
            return this.getCurrSquare().equals(bishop.getCurrSquare()) && this.getColor() == bishop.getColor();
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
