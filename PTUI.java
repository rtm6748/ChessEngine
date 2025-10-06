import java.util.Scanner;

public class PTUI {


    public static void main(String[] args) {
        ChessGame game = new ChessGame();
        System.out.println(game.getGameBoard());
        System.out.println("Please enter your next move, in the form of xBegin yBegin xEnd yEnd");
        Scanner scanner = new Scanner(System.in);
        String nextMove = scanner.nextLine();
        String[] nextMoveSplit = nextMove.split("\\s+");
        int[] intMove = {0, 0, 0, 0};
        for (int i = 0; i < 4; ++i) {
            intMove[i] = Integer.parseInt(nextMoveSplit[i]);
        }
        Piece currPiece = game.getPiece(new Square(intMove[0], intMove[1]));
        boolean success = game.move(currPiece, new Square(intMove[2], intMove[3]));
    }
}
