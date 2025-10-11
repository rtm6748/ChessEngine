package ChessEngineJava;

import java.util.ArrayList;

public class Empty extends Piece {

    public Empty(Square square, Color color) {
        super(square, color);
    }

    public Empty getCopy() {
        return new Empty(new Square(this.getCurrSquare().getX(), this.getCurrSquare().getY()), this.getColor());
    }

    @Override
    public ArrayList<Square> getMoves(GameBoard currBoard, Square square) {
        return new ArrayList<>();
    }

    @Override
    public double getValue(GameBoard gameBoard, ArrayList<Square> moves) {
        return 0;
    }


    @Override
    public String toString() {
        return getColor() == Color.WHITE ? "__" : "--";
        //return this.getColor() == ChessEngineJava.Color.WHITE ? "■" : "□";
    }

    /*@Override
    public boolean equals(Object obj) {
        if (obj instanceof ChessEngineJava.Empty empty) {
            return this.getCurrSquare().equals(empty.getCurrSquare()) && this.getColor() == empty.getColor();
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
