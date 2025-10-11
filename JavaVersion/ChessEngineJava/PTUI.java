package ChessEngineJava;

import java.util.Scanner;

public class PTUI {

    public static void main(String[] args) {
        ChessGame game = new ChessGame();
        System.out.println(game.getGameBoard());

        while (true) {
            boolean success = false;
            boolean winner = false;
            while (!success) {
                if (game.getGameBoard().getValidMoves(game.getCurrentColor()).isEmpty()) {
                    Color winColor = game.getCurrentColor() == Color.WHITE ? Color.BLACK : Color.WHITE;
                    System.out.println("Game over, " +  winColor + " wins");
                    winner = true;
                    break;
                }
                if (game.getCurrentColor() == Color.BLACK) {
                    System.out.println("Please enter your next move, in the form of xBegin yBegin xEnd yEnd");
                    Scanner scanner = new Scanner(System.in);
                    String nextMove = scanner.nextLine();
                    if (nextMove.equals("eng")) {
                        Engine engine = new Engine(game);
                        Move move = engine.findNextMove();
                        game.move(move.getPiece(), move.getSquare());
                        System.out.println(game.getGameBoard());
                        break;
                    }
                    String[] nextMoveSplit = nextMove.strip().split("\\s+");
                    int[] intMove = {0, 0, 0, 0};
                    for (int i = 0; i < 4; ++i) {
                        intMove[i] = Integer.parseInt(nextMoveSplit[i]);
                    }
                    Piece currPiece = game.getPiece(new Square(intMove[0], intMove[1]));
                    success = game.move(currPiece, new Square(intMove[2], intMove[3]));
                    if (success) {
                        System.out.println(game.getGameBoard());
                    } else {
                        System.out.println(game.getGameBoard().getPiece(new Square(intMove[0], intMove[1])));
                        System.out.println(game.getGameBoard().getPiece(new Square(intMove[2], intMove[3])));
                        System.out.println("Invalid ChessEngineJava.Move Please try again");
                    }
                }
                else {
                    Engine engine = new Engine(game);
                    Move move = engine.findNextMove();
                    game.move(move.getPiece(), move.getSquare());
                    System.out.println(game.getGameBoard());
                }
            }
            if (winner) {
                break;
            }
        }
    }
}

