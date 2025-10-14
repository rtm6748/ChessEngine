// ChessEngineCpp.cpp : Defines the entry point for the application.
//

#include "ChessEngineCpp.h"
#include <array>
#include <bit>
#include <chrono>
#include <random>

//All squares except the left edge of the chess board
constexpr uint64_t NOTLEFTEDGE{ 0b11111110'11111110'11111110'11111110'11111110'11111110'11111110'11111110 };
//All squares except the right edge of the chess board
constexpr uint64_t NOTRIGHTEDGE{ 0b01111111'01111111'01111111'01111111'01111111'01111111'01111111'01111111 };
//The third row of the chess board
constexpr uint64_t FOURTHROW{ 0b00000000'00000000'00000000'00000000'11111111'00000000'00000000'00000000 };
//The sixth row of the chess board
constexpr uint64_t FIFTHROW{ 0b00000000'00000000'00000000'11111111'00000000'00000000'00000000'00000000 };


/// <summary>
/// All pieces on a chess board for both white and black
/// including pawns, knights, rooks, queens, king,
/// all white pieces, all black pieces
/// castling rights, enPassant, and white to move
/// </summary>
struct ChessBoard {
	std::uint64_t whitePawns{ 0 };
	std::uint64_t whiteKnights{ 0 };
	std::uint64_t whiteBishops{ 0 };
	std::uint64_t whiteRooks{ 0 };
	std::uint64_t whiteQueens{ 0 };
	std::uint64_t whiteKing{ 0 };

	std::uint64_t blackPawns{ 0 };
	std::uint64_t blackKnights{ 0 };
	std::uint64_t blackBishops{ 0 };
	std::uint64_t blackRooks{ 0 };
	std::uint64_t blackQueens{ 0 };
	std::uint64_t blackKing{ 0 };

	std::uint64_t whitePieces{ 0 };
	std::uint64_t blackPieces{ 0 };

	std::uint8_t castlingRights{ 0 }; //0000 WQ WK BQ BK 0b00001111 1 = true, last 4 bits only
	std::uint8_t enPassantSquare{ 0 }; // 0-63 = square, 64 = not valid

	bool whiteToMove{ 0 };
};

/// <summary>
/// Places pieces of an empty chess board into the correct starting locations for a real game
/// includes setup for white to move, enPassant false, and castling rights to true
/// </summary>
/// <param name="emptyChessBoard">chessBoard that is set to starting location</param>
void makeNewChessBoard(ChessBoard & emptyChessBoard) {
	emptyChessBoard.whitePawns = 0b00000000'00000000'00000000'00000000'00000000'00000000'11111111'00000000;
	emptyChessBoard.whiteKnights = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'01000010;
	emptyChessBoard.whiteBishops = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00100100;
	emptyChessBoard.whiteRooks = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000001;
	emptyChessBoard.whiteQueens = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00001000;
	emptyChessBoard.whiteKing = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00010000;

	emptyChessBoard.blackPawns = 0b00000000'11111111'00000000'00000000'00000000'00000000'00000000'00000000;
	emptyChessBoard.blackKnights = 0b01000010'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
	emptyChessBoard.blackBishops = 0b00100100'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
	emptyChessBoard.blackRooks = 0b10000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
	emptyChessBoard.blackQueens = 0b00001000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
	emptyChessBoard.blackKing = 0b00010000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;

	emptyChessBoard.whitePieces = 0b00000000'00000000'00000000'00000000'00000000'00000000'11111111'11111111;
	emptyChessBoard.blackPieces = 0b11111111'11111111'00000000'00000000'00000000'00000000'00000000'00000000;

	emptyChessBoard.castlingRights = 0b00001111;
	emptyChessBoard.enPassantSquare = 64;

	emptyChessBoard.whiteToMove = true;
}

/// <summary>
/// Updates whitePieces, and blackPieces in chessBoard,
/// must be called every time a piece in chessBoard is moved
/// </summary>
/// <param name="chessBoard">ChessBoard that whitePieces, and blackPieces are updated for</param>
void updateChessBoardPieces(ChessBoard & chessBoard) {
	chessBoard.whitePieces = chessBoard.whitePawns | chessBoard.whiteKnights | chessBoard.whiteBishops | chessBoard.whiteRooks | chessBoard.whiteQueens | chessBoard.whiteKing;
	chessBoard.blackPieces = chessBoard.blackPawns | chessBoard.blackKnights | chessBoard.blackBishops | chessBoard.blackRooks | chessBoard.blackQueens | chessBoard.blackKing;
}

/// <summary>
/// Gets valid pawn moves for 1 color of pieces, including captures, push forward 1, push forward 2, and enPassant 
/// Does not mean that the move is valid, just possible.
/// </summary>
/// <param name="chessBoard">The chess board that moves are generated for</param>
/// <param name="whiteToMove">true if moves are generated for white, false for black</param>
/// <returns>std::uint64_t bitmap where 1 represents valid move, 0 represents not valid</returns>
std::uint64_t getPawnMoves(const ChessBoard & chessBoard, bool whiteToMove) {
	std::uint64_t validPawnMoves{ 0ULL };
	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	std::uint64_t otherPieces = whiteToMove ? chessBoard.blackPieces : chessBoard.whitePieces;
	std::uint64_t empty = ~(currPieces | otherPieces);
	std::uint64_t pawns = whiteToMove ? chessBoard.whitePawns : chessBoard.blackPawns;

	if (chessBoard.enPassantSquare != 64) {
		otherPieces |= 1ULL << chessBoard.enPassantSquare;
	}

	if (whiteToMove) {
		std::uint64_t firstMove{ (pawns << 8) & empty };
		validPawnMoves |= firstMove;
		validPawnMoves |= (firstMove << 8) & empty & FOURTHROW;
		validPawnMoves |= ((pawns << 9) & NOTLEFTEDGE & otherPieces) | ((pawns << 7) & NOTRIGHTEDGE & otherPieces);
	}
	else {
		std::uint64_t firstMove{ (pawns >> 8) & empty };
		validPawnMoves |= firstMove;
		validPawnMoves |= (firstMove >> 8) & empty & FIFTHROW;
		validPawnMoves |= ((pawns >> 9) & NOTRIGHTEDGE & otherPieces) | ((pawns >> 7) & NOTLEFTEDGE & otherPieces);
	}

	return validPawnMoves;

}

/// <summary>
/// gets the valid King moves for 1 color of king
/// does not mean the move is valid, just possible.
/// </summary>
/// <param name="chessBoard">The chess board that moves are generated for</param>
/// <param name="whiteToMove">true if moves are generated for white, false for black</param>
/// <returns>std::uint64_t bitmap where 1 represents valid move, 0 represents not valid</returns>
std::uint64_t getKingMoves(const ChessBoard& chessBoard, bool whiteToMove) {
	std::uint64_t currKing = whiteToMove ? chessBoard.whiteKing : chessBoard.blackKing;
	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	return (currKing << 8 | currKing >> 8 | (currKing << 1 & NOTLEFTEDGE) |
		(currKing >> 1 & NOTRIGHTEDGE) | (currKing << 9 & NOTLEFTEDGE) |
		(currKing << 7 & NOTRIGHTEDGE) | (currKing >> 9 & NOTRIGHTEDGE) |
		(currKing >> 7 & NOTLEFTEDGE)) & ~currPieces;
}

/// <summary>
/// Makes a string representation of chessBoard including colors for both pieces
/// </summary>
/// <param name="chessBoard">The chessBoard that string representaion is generated for</param>
/// <returns>std::string string representation of chessBoard</returns>
std::string chessBoardToString(const ChessBoard & chessBoard) {
	std::string board{};
	std::string whiteColor{ "\033[35m" };
	std::string blackColor{ "\033[36m" };
	std::string reset{ "\033[0m" };
	for (int i = 7; i >= 0; --i) {
		for (int j = 0; j < 8; ++j) {
			int square = 8 * i + j;
			std::uint64_t mask = 1ULL << square;
			if (mask & chessBoard.whitePawns) board.append(whiteColor + "P " + reset);
			else if (mask & chessBoard.blackPawns) board.append(blackColor + "P " + reset);
			else if (mask & chessBoard.whiteKnights) board.append(whiteColor + "N " + reset);
			else if (mask & chessBoard.blackKnights) board.append(blackColor + "N " + reset);
			else if (mask & chessBoard.whiteBishops) board.append(whiteColor + "B " + reset);
			else if (mask & chessBoard.blackBishops) board.append(blackColor + "B " + reset);
			else if (mask & chessBoard.whiteRooks) board.append(whiteColor + "R " + reset);
			else if (mask & chessBoard.blackRooks) board.append(blackColor + "R " + reset);
			else if (mask & chessBoard.whiteQueens) board.append(whiteColor + "Q " + reset);
			else if (mask & chessBoard.blackQueens) board.append(blackColor + "Q " + reset);
			else if (mask & chessBoard.whiteKing) board.append(whiteColor + "K " + reset);
			else if (mask & chessBoard.blackKing) board.append(blackColor + "K " + reset);
			else board.append("- ");
		}
		board.append("\n");
	}
	return board;
}

int main() {
	
	ChessBoard chessBoard;
	makeNewChessBoard(chessBoard);
	chessBoard.whiteBishops = { 0b00000000'00000000'00000100'01000000'00000000'00000000'00000000'00000000 };
	chessBoard.blackBishops = { 0b00000000'00000000'00000010'10000000'00000000'00000000'00000000'00000000 };
	updateChessBoardPieces(chessBoard);
	std::cout << chessBoardToString(chessBoard) << "\n";
	std::uint64_t validMoves{ getPawnMoves(chessBoard, false) };
	chessBoard.blackPawns = validMoves;
	updateChessBoardPieces(chessBoard);
	std::cout << chessBoardToString(chessBoard) << "\n";

	std::uint64_t validMoves2{ getPawnMoves(chessBoard, false) };
	chessBoard.blackPawns = validMoves2;
	updateChessBoardPieces(chessBoard);
	std::cout << chessBoardToString(chessBoard) << "\n";
	return 0;
}
