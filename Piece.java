import java.util.ArrayList;

public abstract class Piece {
    private final Color color;
    private Square square;

    public Piece(Square square, Color color) {
        this.color = color;
        this.square = square;
    }

    public abstract Piece getCopy();

    public void setSquare(Square square) {
        this.square = square;
    }

    public Square getCurrSquare() {
        return this.square;
    }

    public boolean isValid(Square square) {
        return square.getX() >= 0 && square.getX() < GameBoard.boardSize && square.getY() >= 0 && square.getY() < GameBoard.boardSize;
    }

    public Color getColor() {
        return this.color;
    }

    public abstract ArrayList<Square> getMoves(GameBoard currBoard, Square square);
    public abstract double getValue(GameBoard gameBoard, ArrayList<Square> moves);
}
