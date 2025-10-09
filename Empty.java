import java.util.ArrayList;
import java.util.Objects;

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
        //return this.getColor() == Color.WHITE ? "■" : "□";
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Empty empty) {
            return this.getCurrSquare().equals(empty.getCurrSquare()) && this.getColor() == empty.getColor();
        }
        return false;
    }

    @Override
    public int hashCode() {
        Square s = this.getCurrSquare();
        return Objects.hash(this.getColor(), this.getClass()) + s.hashCode();
    }
}
