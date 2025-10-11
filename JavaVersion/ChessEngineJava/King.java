package ChessEngineJava;

import java.util.ArrayList;

public class King extends Piece {
    public King(Square square, Color color) {
        super(square, color);
    }

    public double getValue(GameBoard gameBoard, ArrayList<Square> moves) {
        double val = 1000;
        double edgePreference = 0.2;
        val += Math.abs(3.5 - this.getCurrSquare().getY()) * edgePreference;
        return val;
    }

    public King getCopy() {
        return new King(new Square(this.getCurrSquare().getX(), this.getCurrSquare().getY()), this.getColor());
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        ArrayList<Square> validMoves = new ArrayList<>();
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) {
                    continue;
                }
                Square newSquare = new Square(square, i, j);
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
        return getColor() == Color.WHITE ? "WK" : "BK";
        //return getColor() == ChessEngineJava.Color.WHITE ? "♚" : "♔";
    }

    /*@Override
    public boolean equals(Object obj) {
        if (obj instanceof ChessEngineJava.King king) {
            return this.getCurrSquare().equals(king.getCurrSquare()) && this.getColor() == king.getColor();
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
