#include <bit>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int MOVESGENERATED = 0;



enum Piece {
	NONE,
	WHITEPAWN,
	WHITEKNIGHT,
	WHITEBISHOP,
	WHITEROOK,
	WHITEQUEEN,
	WHITEKING,
	BLACKPAWN,
	BLACKKNIGHT,
	BLACKBISHOP,
	BLACKROOK,
	BLACKQUEEN,
	BLACKKING,
};

struct Move {
	std::uint64_t from; // square that piece is moved from
	std::uint64_t to; // square that piece is moved to
	Piece capturedPiece;
	Piece promotionPiece;
	std::uint8_t specialMoves; // 000 enPassant WQ WK BQ BK 0b 0001'1111 0 = false, 1 = true
};

//All squares except the left edge of the chess board
constexpr std::uint64_t NOTLEFTEDGE{ 0b11111110'11111110'11111110'11111110'11111110'11111110'11111110'11111110 };
//All squares except the right edge of the chess board
constexpr std::uint64_t NOTRIGHTEDGE{ 0b01111111'01111111'01111111'01111111'01111111'01111111'01111111'01111111 };
//All squares except the 2 left edges of the chess board
constexpr std::uint64_t NOT2LEFTEDGE{ 0b11111100'11111100'11111100'11111100'11111100'11111100'11111100'11111100 };
//All squares except the 2 left edges of the chess board
constexpr std::uint64_t NOT2RIGHTEDGE{ 0b00111111'0011111'00111111'00111111'00111111'00111111'00111111'00111111 };
//The third row of the chess board
constexpr std::uint64_t FOURTHROW{ 0b00000000'00000000'00000000'00000000'11111111'00000000'00000000'00000000 };
//The sixth row of the chess board
constexpr std::uint64_t FIFTHROW{ 0b00000000'00000000'00000000'11111111'00000000'00000000'00000000'00000000 };
//White Promotion Pieces
constexpr Piece WHITEPROMOTIONPIECES[4]{ WHITEKNIGHT, WHITEBISHOP, WHITEROOK, WHITEQUEEN };
//Black Promotion Pieces
constexpr Piece BLACKPROMOTIONPIECES[4]{ BLACKKNIGHT, BLACKBISHOP, BLACKROOK, BLACKQUEEN };
//Castle squares in Move
constexpr std::uint8_t CASTLEMOVES{ 0b0000'1111 };

constexpr std::uint64_t PAWN_CENTER_CONTROL_SQUARES{ 0b00000000'00000000'00000000'00011000'00011000'00000000'00000000'00000000 };

constexpr std::uint64_t KNIGHT_CENTRALIZED_3{ 0b00000000'00000000'00000000'00011000'00011000'00000000'00000000'00000000 };

constexpr std::uint64_t KNIGHT_CENTRALIZED_2{ 0b00000000'00000000'00111100'00100100'00100100'00111100'00000000'00000000 };

constexpr std::uint64_t KNIGHT_CENTRALIZED_1{ 0b00000000'01111110'01000010'01000010'01000010'01000010'01111110'00000000 };

constexpr int PAWN_VALUE{ 100000 };

constexpr int PAWN_DEFEND_PAWN_VALUE{ 10000 };

constexpr int PAWN_DEFEND_PIECE_VALUE{ 10000 };

constexpr int PAWN_ATTACK_PAWN_VALUE{ 10000 };

constexpr int PAWN_ATTACK_PIECE_VALUE{ 10000 };

constexpr int PAWN_EXTRA_RANK_VALUE{ 10000 };

constexpr int PAWN_CENTER_CONTROL_VALUE{ 10000 };

constexpr int KNIGHT_VALUE{ 300000 };

constexpr int KNIGHT_DEFEND_PIECE_VALUE{ 10000 };

constexpr int KNIGHT_DEFEND_PAWN_VALUE{ 10000 };

constexpr int KNIGHT_ATTACK_PIECE_VALUE{ 10000 };

constexpr int KNIGHT_ATTACK_PAWN_VALUE{ 10000 };

constexpr int KNIGHT_CENTRALIZED_VALUE{ 10000 };

constexpr int ROOK_VALUE{ 500000 };

constexpr int ROOK_EMPTY_SQUARE_VALUE{ 10000 };

constexpr int ROOK_DEFEND_PIECE_VALUE{ 10000 };

constexpr int ROOK_DEFEND_PAWN_VALUE{ 10000 };

constexpr int ROOK_ATTACK_PIECE_VALUE{ 10000 };

constexpr int ROOK_ATTACK_PAWN_VALUE{ 10000 };

constexpr int BISHOP_VALUE{ 300000 };

constexpr int BISHOP_EMPTY_SQUARE_VALUE{ 10000 };

constexpr int BISHOP_DEFEND_PIECE_VALUE{ 10000 };

constexpr int BISHOP_DEFEND_PAWN_VALUE{ 10000 };

constexpr int BISHOP_ATTACK_PIECE_VALUE{ 10000 };

constexpr int BISHOP_ATTACK_PAWN_VALUE{ 10000 };

constexpr int QUEEN_VALUE{ 900000 };

constexpr int QUEEN_EMPTY_SQUARE_VALUE{ 10000 };

constexpr int QUEEN_DEFEND_PIECE_VALUE{ 10000 };

constexpr int QUEEN_DEFEND_PAWN_VALUE{ 10000 };

constexpr int QUEEN_ATTACK_PIECE_VALUE{ 10000 };

constexpr int QUEEN_ATTACK_PAWN_VALUE{ 10000 };




Move makeMove(std::uint64_t from = 0ULL, std::uint64_t to = 0ULL, Piece capturedPiece = Piece::NONE, Piece promotionPiece = Piece::NONE, std::uint8_t specialMoves = static_cast<std::uint8_t>(0)) {
	Move move{};
	move.from = from;
	move.to = to;
	move.capturedPiece = capturedPiece;
	move.promotionPiece = promotionPiece;
	move.specialMoves = specialMoves;
	return move;
}


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

	bool whiteToMove{ true };
};

/// <summary>
/// Places pieces of an empty chess board into the correct starting locations for a real game
/// includes setup for white to move, enPassant false, and castling rights to true
/// </summary>
/// <param name="emptyChessBoard">chessBoard that is set to starting location</param>
void makeNewChessBoard(ChessBoard& emptyChessBoard) {
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
void updateChessBoardPieces(ChessBoard& chessBoard) {
	chessBoard.whitePieces = chessBoard.whitePawns | chessBoard.whiteKnights | chessBoard.whiteBishops | chessBoard.whiteRooks | chessBoard.whiteQueens | chessBoard.whiteKing;
	chessBoard.blackPieces = chessBoard.blackPawns | chessBoard.blackKnights | chessBoard.blackBishops | chessBoard.blackRooks | chessBoard.blackQueens | chessBoard.blackKing;
}

/// <summary>
/// Gets valid pawn moves for 1 color of pieces, including captures, push forward 1, push forward 2, and enPassant 
/// Does not mean that the move is valid, just possible.
/// </summary>
/// <param name="chessBoard">The chess board that moves are generated for</param>
/// <returns>std::uint64_t bitmap where 1 represents valid move, 0 represents not valid</returns>
std::uint64_t getPawnMoves(const ChessBoard& chessBoard) {
	bool whiteToMove = chessBoard.whiteToMove;
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
/// <returns>std::uint64_t bitmap where 1 represents valid move, 0 represents not valid</returns>
std::uint64_t getKingMoves(const ChessBoard& chessBoard) {
	bool whiteToMove = chessBoard.whiteToMove;
	std::uint64_t currKing = whiteToMove ? chessBoard.whiteKing : chessBoard.blackKing;
	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	return (currKing << 8 | currKing >> 8 | (currKing << 1 & NOTLEFTEDGE) |
		(currKing >> 1 & NOTRIGHTEDGE) | (currKing << 9 & NOTLEFTEDGE) |
		(currKing << 7 & NOTRIGHTEDGE) | (currKing >> 9 & NOTRIGHTEDGE) |
		(currKing >> 7 & NOTLEFTEDGE)) & ~currPieces;
}

/// <summary>
/// gets the valid Rook moves for 1 color of rooks, includes all moves for all rooks
/// does not mean that the moves is valid, just possible.
/// </summary>
/// <param name="chessBoard">The chess board that moves are generated for</param>
/// <returns>std::uint64_t bitmap where 1 represents valid move, 0 represents not valid</returns>
std::uint64_t getRookMoves(const ChessBoard& chessBoard) {
	bool whiteToMove = chessBoard.whiteToMove;
	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	std::uint64_t otherPieces = whiteToMove ? chessBoard.blackPieces : chessBoard.whitePieces;
	std::uint64_t currRooks = whiteToMove ? chessBoard.whiteRooks : chessBoard.blackRooks;
	std::uint64_t validRookMoves{ 0ULL };

	while (currRooks) {
		int thisRook = std::countr_zero(currRooks); // get current rook
		std::uint64_t rook{ 1ULL << thisRook };

		bool valid{ true };
		int shift{ 1 };

		while (valid) { // left
			std::uint64_t thisMove{ (rook & NOTLEFTEDGE) >> shift };
			validRookMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			++shift;
		}

		valid = true;
		shift = 1;

		while (valid) { // right
			std::uint64_t thisMove{ (rook & NOTRIGHTEDGE) << shift };
			validRookMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			++shift;
		}

		valid = true;
		shift = 8;

		while (valid) { // up
			std::uint64_t thisMove{ rook << shift };
			validRookMoves |= thisMove & ~currPieces;
			valid = (thisMove) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on board, or if capture, or if in currPieces
			shift += 8;
		}

		valid = true;
		shift = 8;

		while (valid) { // down
			std::uint64_t thisMove{ rook >> shift };
			validRookMoves |= thisMove & ~currPieces;
			valid = (thisMove) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on board, or if capture, or if in currPieces
			shift += 8;
		}
		currRooks ^= 1ULL << thisRook; // remove thisRook from board
	}
	return validRookMoves;
}

/// <summary>
/// gets the valid Queen moves for 1 color of queen, includes all moves for all queens
/// does not mean that the moves is valid, just possible.
/// </summary>
/// <param name="chessBoard">The chess board that moves are generated for</param>
/// <returns>std::uint64_t bitmap where 1 represents valid move, 0 represents not valid</returns>
std::uint64_t getQueenMoves(const ChessBoard& chessBoard) {
	bool whiteToMove = chessBoard.whiteToMove;
	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	std::uint64_t otherPieces = whiteToMove ? chessBoard.blackPieces : chessBoard.whitePieces;
	std::uint64_t currQueens = whiteToMove ? chessBoard.whiteQueens : chessBoard.blackQueens;
	std::uint64_t validQueenMoves{ 0ULL };

	while (currQueens) {
		int thisQueen = std::countr_zero(currQueens); // get current queen
		std::uint64_t queen{ 1ULL << thisQueen };

		bool valid{ true };
		int shift{ 7 };

		while (valid) { // down left
			std::uint64_t thisMove{ (queen & NOTRIGHTEDGE) >> shift };
			validQueenMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 7;
		}

		valid = true;
		shift = 9;

		while (valid) { // up right
			std::uint64_t thisMove{ (queen & NOTRIGHTEDGE) << shift };
			validQueenMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 9;
		}

		valid = true;
		shift = 7;

		while (valid) { // up left
			std::uint64_t thisMove{ (queen & NOTLEFTEDGE) << shift };
			validQueenMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 7;
		}

		valid = true;
		shift = 9;

		while (valid) { // down right
			std::uint64_t thisMove{ (queen & NOTLEFTEDGE) >> shift };
			validQueenMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 9;
		}

		valid = true;
		shift = 1;

		while (valid) { // left
			std::uint64_t thisMove{ (queen & NOTLEFTEDGE) >> shift };
			validQueenMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			++shift;
		}

		valid = true;
		shift = 1;

		while (valid) { // right
			std::uint64_t thisMove{ (queen & NOTRIGHTEDGE) << shift };
			validQueenMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			++shift;
		}

		valid = true;
		shift = 8;

		while (valid) { // up
			std::uint64_t thisMove{ queen << shift };
			validQueenMoves |= thisMove & ~currPieces;
			valid = (thisMove) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on board, or if capture, or if in currPieces
			shift += 8;
		}

		valid = true;
		shift = 8;

		while (valid) { // down
			std::uint64_t thisMove{ queen >> shift };
			validQueenMoves |= thisMove & ~currPieces;
			valid = (thisMove) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on board, or if capture, or if in currPieces
			shift += 8;
		}
		currQueens ^= 1ULL << thisQueen; // remove thisQueen from board
	}
	return validQueenMoves;
}

/// <summary>
/// gets the valid Bishop moves for 1 color of bishop, includes all moves for all bishops
/// does not mean that the moves is valid, just possible.
/// </summary>
/// <param name="chessBoard">The chess board that moves are generated for</param>
/// <returns>std::uint64_t bitmap where 1 represents valid move, 0 represents not valid</returns>
std::uint64_t getBishopMoves(const ChessBoard& chessBoard) {
	bool whiteToMove = chessBoard.whiteToMove;
	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	std::uint64_t otherPieces = whiteToMove ? chessBoard.blackPieces : chessBoard.whitePieces;
	std::uint64_t currBishops = whiteToMove ? chessBoard.whiteBishops : chessBoard.blackBishops;
	std::uint64_t validBishopMoves{ 0ULL };

	while (currBishops) {
		int thisBishop = std::countr_zero(currBishops); // get current bishop
		std::uint64_t bishop{ 1ULL << thisBishop };

		bool valid{ true };
		int shift{ 7 };

		while (valid) { // down left
			std::uint64_t thisMove{ (bishop & NOTRIGHTEDGE) >> shift };
			validBishopMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 7;
		}

		valid = true;
		shift = 9;

		while (valid) { // up right
			std::uint64_t thisMove{ (bishop & NOTRIGHTEDGE) << shift };
			validBishopMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 9;
		}

		valid = true;
		shift = 7;

		while (valid) { // up left
			std::uint64_t thisMove{ (bishop & NOTLEFTEDGE) << shift };
			validBishopMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 7;
		}

		valid = true;
		shift = 9;

		while (valid) { // down right
			std::uint64_t thisMove{ (bishop & NOTLEFTEDGE) >> shift };
			validBishopMoves |= thisMove & ~currPieces;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 9;
		}
		currBishops ^= 1ULL << thisBishop; // remove thisBishop from board
	}
	return validBishopMoves;
}

/// <summary>
/// gets the valid knight moves for 1 color of knight, includes all moves for all knights
/// does not mean that the moves is valid, just possible.
/// </summary>
/// <param name="chessBoard">The chess board that moves are generated for</param>
/// <returns>std::uint64_t bitmap where 1 represents valid move, 0 represents not valid</returns>
std::uint64_t getKnightMoves(const ChessBoard& chessBoard) {
	bool whiteToMove = chessBoard.whiteToMove;
	std::uint64_t notcurrPieces = whiteToMove ? ~chessBoard.whitePieces : ~chessBoard.blackPieces;
	std::uint64_t currKnights = whiteToMove ? chessBoard.whiteKnights : chessBoard.blackKnights;
	std::uint64_t validKnightMoves{ 0ULL };

	validKnightMoves |= (currKnights & NOT2LEFTEDGE) << 6 & notcurrPieces;
	validKnightMoves |= (currKnights & NOT2RIGHTEDGE) >> 6 & notcurrPieces;
	validKnightMoves |= (currKnights & NOT2RIGHTEDGE) << 10 & notcurrPieces;
	validKnightMoves |= (currKnights & NOT2LEFTEDGE) >> 10 & notcurrPieces;
	validKnightMoves |= (currKnights & NOTLEFTEDGE) << 15 & notcurrPieces;
	validKnightMoves |= (currKnights & NOTRIGHTEDGE) >> 15 & notcurrPieces;
	validKnightMoves |= (currKnights & NOTRIGHTEDGE) << 17 & notcurrPieces;
	validKnightMoves |= (currKnights & NOTLEFTEDGE) >> 17 & notcurrPieces;
	return validKnightMoves;
}

/// <summary>
/// Makes a string representation of chessBoard including colors for both pieces
/// </summary>
/// <param name="chessBoard">The chessBoard that string representaion is generated for</param>
/// <returns>std::string string representation of chessBoard</returns>
std::string chessBoardToString(const ChessBoard& chessBoard) {
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

/// <summary>
/// gets the piece that is on the square of the chessboard
/// </summary>
/// <param name="chessBoard">the chessboard that the piece is taken from</param>
/// <param name="square">the square of the chessboard that the piece is gotten from</param>
/// <returns>Piece that is on the square of the chessboard</returns>
Piece getPiece(const ChessBoard& chessBoard, std::uint64_t square) {
	if (square & chessBoard.whitePieces) {
		if (square & chessBoard.whitePawns) return Piece::WHITEPAWN;
		if (square & chessBoard.whiteKnights) return Piece::WHITEKNIGHT;
		if (square & chessBoard.whiteBishops) return Piece::WHITEBISHOP;
		if (square & chessBoard.whiteRooks) return Piece::WHITEROOK;
		if (square & chessBoard.whiteQueens) return Piece::WHITEQUEEN;
		if (square & chessBoard.whiteKing) return Piece::WHITEKING;
	}
	else if (square & chessBoard.blackPieces) {
		if (square & chessBoard.blackPawns) return Piece::BLACKPAWN;
		if (square & chessBoard.blackKnights) return Piece::BLACKKNIGHT;
		if (square & chessBoard.blackBishops) return Piece::BLACKBISHOP;
		if (square & chessBoard.blackRooks) return Piece::BLACKROOK;
		if (square & chessBoard.blackQueens) return Piece::BLACKQUEEN;
		if (square & chessBoard.blackKing) return Piece::BLACKKING;
	}
	return Piece::NONE;
}

/// <summary>
/// NEED TO CHECK SPECIAL MOVES Generates all possible moves for the current turn of chessBoard,
/// does not mean these are valid, only possible
/// </summary>
/// <param name="chessBoard">the chessBoard that moves are generated from</param>
/// <returns>a vector containing all possible moves for the current position</returns>
std::vector<Move> generateMoves(ChessBoard& chessBoard) {
	bool whiteToMove{ chessBoard.whiteToMove };
	std::vector<Move> moves{};
	std::uint64_t intMove{};

	{
		std::uint64_t& chessBoardPawn{ whiteToMove ? chessBoard.whitePawns : chessBoard.blackPawns };
		std::uint64_t originalPawns{ chessBoardPawn };
		std::uint64_t modifyPawns{ originalPawns };
		std::uint64_t currPawn{ 0 };

		while (modifyPawns) {
			currPawn = std::countr_zero(modifyPawns);
			chessBoardPawn = 1ULL << currPawn;
			intMove = getPawnMoves(chessBoard);
			while (intMove) {
				int currMoveNum{ std::countr_zero(intMove) };
				std::uint64_t currMove{ 1ULL << currMoveNum };
				if (std::countr_zero(intMove) > 54 || currMoveNum < 8) { //promotion
					if (chessBoard.whiteToMove) {
						for (Piece piece : WHITEPROMOTIONPIECES) {
							moves.emplace_back(makeMove(chessBoardPawn, currMove, getPiece(chessBoard, currMove), piece)); // think I need to fix to, and cpatured piece????
						}
					}
					else {
						for (Piece piece : BLACKPROMOTIONPIECES) {
							moves.emplace_back(makeMove(chessBoardPawn, currMove, getPiece(chessBoard, currMove), piece));
						}
					}
					
				}
				else {
					moves.emplace_back(makeMove(chessBoardPawn, currMove, getPiece(chessBoard, currMove), Piece::NONE));
				}
				intMove &= intMove - 1;
			}
			modifyPawns &= modifyPawns - 1;
		}
		chessBoardPawn = originalPawns;
	}
	{
		std::uint64_t& chessBoardKnight{ whiteToMove ? chessBoard.whiteKnights : chessBoard.blackKnights };
		std::uint64_t originalKnights{ chessBoardKnight };
		std::uint64_t modifyKnights{ originalKnights };
		std::uint64_t currKnight{ 0 };

		while (modifyKnights) {
			currKnight = std::countr_zero(modifyKnights);
			chessBoardKnight = 1ULL << currKnight;
			intMove = getKnightMoves(chessBoard);
			while (intMove) {
				std::uint64_t currMove{ 1ULL << std::countr_zero(intMove)};
				moves.emplace_back(makeMove(chessBoardKnight, currMove, getPiece(chessBoard, currMove), Piece::NONE));
				intMove &= intMove - 1;
			}
			modifyKnights &= modifyKnights - 1;
		}
		chessBoardKnight = originalKnights;
	}
	{
		std::uint64_t& chessBoardBishop{ whiteToMove ? chessBoard.whiteBishops : chessBoard.blackBishops };
		std::uint64_t originalBishops{ chessBoardBishop };
		std::uint64_t modifyBishops{ originalBishops };
		std::uint64_t currBishop{ 0 };

		while (modifyBishops) {
			currBishop = std::countr_zero(modifyBishops);
			chessBoardBishop = 1ULL << currBishop;
			intMove = getBishopMoves(chessBoard);
			while (intMove) {
				std::uint64_t currMove{ 1ULL << std::countr_zero(intMove) };
				moves.emplace_back(makeMove(chessBoardBishop, currMove, getPiece(chessBoard, currMove), Piece::NONE));
				intMove &= intMove - 1;
			}
			modifyBishops &= modifyBishops - 1;
		}
		chessBoardBishop = originalBishops;
	}
	{
		std::uint64_t& chessBoardRook{ whiteToMove ? chessBoard.whiteRooks : chessBoard.blackRooks };
		std::uint64_t originalRooks{ chessBoardRook };
		std::uint64_t modifyRooks{ originalRooks };
		std::uint64_t currRook{ 0 };

		while (modifyRooks) {
			currRook = std::countr_zero(modifyRooks);
			chessBoardRook = 1ULL << currRook;
			intMove = getRookMoves(chessBoard);
			while (intMove) {
				std::uint64_t currMove{ 1ULL << std::countr_zero(intMove) };
				moves.emplace_back(makeMove(chessBoardRook, currMove, getPiece(chessBoard, currMove), Piece::NONE));
				intMove &= intMove - 1;
			}
			modifyRooks &= modifyRooks - 1;
		}
		chessBoardRook = originalRooks;
	}
	{
		std::uint64_t& chessBoardQueen{ whiteToMove ? chessBoard.whiteQueens : chessBoard.blackQueens };
		std::uint64_t originalQueens{ chessBoardQueen };
		std::uint64_t modifyQueens{ originalQueens };
		std::uint64_t currQueen{ 0 };

		while (modifyQueens) {
			currQueen = std::countr_zero(modifyQueens);
			chessBoardQueen = 1ULL << currQueen;
			intMove = getQueenMoves(chessBoard);
			while (intMove) {
				std::uint64_t currMove{ 1ULL << std::countr_zero(intMove) };
				moves.emplace_back(makeMove(chessBoardQueen, currMove, getPiece(chessBoard, currMove), Piece::NONE));
				intMove &= intMove - 1;
			}
			modifyQueens &= modifyQueens - 1;
		}
		chessBoardQueen = originalQueens;
	}
	{
		std::uint64_t& chessBoardKing{ whiteToMove ? chessBoard.whiteKing : chessBoard.blackKing };
		std::uint64_t originalKings{ chessBoardKing };
		std::uint64_t modifyKings{ originalKings };
		std::uint64_t currKing{ 0 };

		while (modifyKings) {
			currKing = std::countr_zero(modifyKings);
			chessBoardKing = 1ULL << currKing;
			intMove = getKingMoves(chessBoard);
			while (intMove) {
				std::uint64_t currMove{ 1ULL << std::countr_zero(intMove) };
				moves.emplace_back(makeMove(chessBoardKing, currMove, getPiece(chessBoard, currMove), Piece::NONE));
				intMove &= intMove - 1;
			}
			modifyKings &= modifyKings - 1;
		}
		chessBoardKing = originalKings;
	}

	return moves;
}

/// <summary>
/// NEED TO CHECK promotes a pawn to any piece
/// </summary>
/// <param name="chessBoard">the chessboard that piece is promoted on</param>
/// <param name="move"> the move containing the promotion piece that will be placed on the board</param>
void pawnPromotion(ChessBoard& chessBoard, const Move& move) {
	switch (move.promotionPiece)
	{
	case WHITEKNIGHT:
		chessBoard.whiteKnights |= move.to;
		break;
	case WHITEROOK:
		chessBoard.whiteRooks |= move.to;
		break;
	case WHITEBISHOP:
		chessBoard.whiteBishops |= move.to;
		break;
	case WHITEQUEEN:
		chessBoard.whiteQueens |= move.to;
		break;
	case BLACKKNIGHT:
		chessBoard.blackKnights |= move.to;
		break;
	case BLACKROOK:
		chessBoard.blackRooks |= move.to;
		break;
	case BLACKBISHOP:
		chessBoard.blackBishops |= move.to;
		break;
	case BLACKQUEEN:
		chessBoard.blackQueens |= move.to;
		break;
	}
}

/// <summary>
/// NEED TO CHECK Sets piece on first square to piece on second square of move, and captures any piece that may be on that square
/// if castles, castles rook and knight
/// if enpassant, captures pawn
/// changes white to move to oppisite color when finished
/// </summary>
/// <param name="chessBoard">the chessBoard that peice is moved on</param>
/// <param name="move">Move that is carried out on the chess board</param>
void makeMove(ChessBoard& chessBoard, const Move& move) {
	std::uint8_t castle{ static_cast<std::uint8_t>(CASTLEMOVES & move.specialMoves) };

	if (move.specialMoves | castle) {
		switch (move.specialMoves)
		{
		case static_cast<std::uint8_t>(1): // black king
			chessBoard.blackKing = chessBoard.blackKing << 2;
			chessBoard.blackRooks ^= 1ULL << 61 & 1ULL << 63;
			break;
		case static_cast<std::uint8_t>(1) << 1: // black queen
			chessBoard.blackKing = chessBoard.blackKing >> 2;
			chessBoard.blackRooks ^= 1ULL << 59 & 1ULL << 56;
			break;
		case static_cast<std::uint8_t>(1) << 2: // white king
			chessBoard.whiteKing = chessBoard.whiteKing << 2;
			chessBoard.whiteRooks ^= 1ULL << 5 & 1ULL << 7;
			break;
		case static_cast<std::uint8_t>(1) << 3: // white queen
			chessBoard.whiteKing = chessBoard.whiteKing >> 2;
			chessBoard.whiteRooks ^= 1ULL << 3 & 1ULL;
			break;
		}
		chessBoard.whiteToMove = !chessBoard.whiteToMove;
		updateChessBoardPieces(chessBoard);
		return;
	}

	if (move.specialMoves) {
		bool white = chessBoard.whiteToMove;
		if (white) {
			chessBoard.blackPawns ^= (move.to >> 8) & move.to; // flips between pawn, and capture square
		}
		else {
			chessBoard.whitePawns ^= (move.to << 8) & move.to; // flips between pawn, and capture square
		}
	}

	Piece type{ getPiece(chessBoard, move.from) };
	Piece othertype{ getPiece(chessBoard, move.to) };

	switch (othertype) // captures pice by removing it from the board
	{
	case WHITEPAWN:
		chessBoard.whitePawns ^= move.to;
		break;
	case WHITEKNIGHT:
		chessBoard.whiteKnights ^= move.to;
		break;
	case WHITEBISHOP:
		chessBoard.whiteBishops ^= move.to;
		break;
	case WHITEROOK:
		chessBoard.whiteRooks ^= move.to;
		break;
	case WHITEQUEEN:
		chessBoard.whiteQueens ^= move.to;
		break;
	case WHITEKING:
		chessBoard.whiteKing ^= move.to;
		break;
	case BLACKPAWN:
		chessBoard.blackPawns ^= move.to;
		break;
	case BLACKKNIGHT:
		chessBoard.blackKnights ^= move.to;
		break;
	case BLACKBISHOP:
		chessBoard.blackBishops ^= move.to;
		break;
	case BLACKROOK:
		chessBoard.blackRooks ^= move.to;
		break;
	case BLACKQUEEN:
		chessBoard.blackQueens ^= move.to;
		break;
	case BLACKKING:
		chessBoard.blackKing ^= move.to;
		break;
	}

	switch (type) // moves piece to new square
	{
	case WHITEPAWN:
		if (move.promotionPiece) {
			pawnPromotion(chessBoard, move);
			chessBoard.whitePawns ^= move.from;
		}
		else {
			chessBoard.whitePawns = (chessBoard.whitePawns ^ move.from) | move.to;
		}
		break;
	case WHITEKNIGHT:
		chessBoard.whiteKnights = (chessBoard.whiteKnights ^ move.from) | move.to;
		break;
	case WHITEBISHOP:
		chessBoard.whiteBishops = (chessBoard.whiteBishops ^ move.from) | move.to;
		break;
	case WHITEROOK:
		chessBoard.whiteRooks = (chessBoard.whiteRooks ^ move.from) | move.to;
		break;
	case WHITEQUEEN:
		chessBoard.whiteQueens = (chessBoard.whiteQueens ^ move.from) | move.to;
		break;
	case WHITEKING:
		chessBoard.whiteKing = (chessBoard.whiteKing ^ move.from) | move.to;
		break;
	case BLACKPAWN:
		if (move.promotionPiece) {
			pawnPromotion(chessBoard, move);
			chessBoard.blackPawns ^= move.from;
		}
		else {
			chessBoard.blackPawns = (chessBoard.blackPawns ^ move.from) | move.to;
		}
		break;
	case BLACKKNIGHT:
		chessBoard.blackKnights = (chessBoard.blackKnights ^ move.from) | move.to;
		break;
	case BLACKBISHOP:
		chessBoard.blackBishops = (chessBoard.blackBishops ^ move.from) | move.to;
		break;
	case BLACKROOK:
		chessBoard.blackRooks = (chessBoard.blackRooks ^ move.from) | move.to;
		break;
	case BLACKQUEEN:
		chessBoard.blackQueens = (chessBoard.blackQueens ^ move.from) | move.to;
		break;
	case BLACKKING:
		chessBoard.blackKing = (chessBoard.blackKing ^ move.from) | move.to;
		break;
	}
	chessBoard.whiteToMove = !chessBoard.whiteToMove;
	updateChessBoardPieces(chessBoard);
}
	
/// <summary>
/// NEED TO CHECK unpromotes a piece to a pawn
/// </summary>
/// <param name="chessBoard">the chessBoard that piece is unmoved from</param>
/// <param name="move">the move that is undone</param>
void unPromote(ChessBoard& chessBoard, Move& move) {
	switch (move.promotionPiece)
	{
	case WHITEKNIGHT:
		chessBoard.whiteKnights ^= move.to;
		break;
	case WHITEROOK:
		chessBoard.whiteRooks ^= move.to;
		break;
	case WHITEBISHOP:
		chessBoard.whiteBishops ^= move.to;
		break;
	case WHITEQUEEN:
		chessBoard.whiteQueens ^= move.to;
		break;
	case BLACKKNIGHT:
		chessBoard.blackKnights ^= move.to;
		break;
	case BLACKROOK:
		chessBoard.blackRooks ^= move.to;
		break;
	case BLACKBISHOP:
		chessBoard.blackBishops ^= move.to;
		break;
	case BLACKQUEEN:
		chessBoard.blackQueens ^= move.to;
		break;
	}
}

/// <summary>
/// NEED TO CHECK unmoves a piece on the chessBoard,
/// undoes castles, undoes captures, undoes enPassant
/// changes white to move to the original color
/// </summary>
/// <param name="chessBoard">the chessBoard that move is undone from</param>
/// <param name="move">the move that is undone</param>
void unMove(ChessBoard& chessBoard, Move& move) {
	chessBoard.whiteToMove = !chessBoard.whiteToMove;
	std::uint8_t castle{ static_cast<std::uint8_t>(CASTLEMOVES & move.specialMoves) };

	//undo castle
	if (move.specialMoves | castle) {
		switch (move.specialMoves)
		{
		case static_cast<std::uint8_t>(1): // black king
			chessBoard.blackKing = chessBoard.blackKing >> 2;
			chessBoard.blackRooks ^= 1ULL << 61 & 1ULL << 63;
			break;
		case static_cast<std::uint8_t>(1) << 1: // black queen
			chessBoard.blackKing = chessBoard.blackKing >> 2;
			chessBoard.blackRooks ^= 1ULL << 59 & 1ULL << 56;
			break;
		case static_cast<std::uint8_t>(1) << 2: // white king
			chessBoard.whiteKing = chessBoard.whiteKing >> 2;
			chessBoard.whiteRooks ^= 1ULL << 5 & 1ULL << 7;
			break;
		case static_cast<std::uint8_t>(1) << 3: // white queen
			chessBoard.whiteKing = chessBoard.whiteKing >> 2;
			chessBoard.whiteRooks ^= 1ULL << 3 & 1ULL;
			break;
		}
		updateChessBoardPieces(chessBoard);
		return;

		
	}
	// undo enPassant

	if (move.specialMoves) {
		bool white = chessBoard.whiteToMove;
		if (white) {
			chessBoard.blackPawns ^= (move.to >> 8) & move.to; // flips between pawn, and capture square
		}
		else {
			chessBoard.whitePawns ^= (move.to << 8) & move.to; // flips between pawn, and capture square
		}
	}


	//undo piece move

	Piece type{ getPiece(chessBoard, move.to) };
	Piece othertype{ move.capturedPiece };

	switch (type) // moves piece back to original square
	{
	case WHITEPAWN:
		chessBoard.whitePawns = (chessBoard.whitePawns ^ move.to) | move.from;
		if (move.promotionPiece) {
			unPromote(chessBoard, move);
		}
		break;
	case WHITEKNIGHT:
		chessBoard.whiteKnights = (chessBoard.whiteKnights ^ move.to) | move.from;
		break;
	case WHITEBISHOP:
		chessBoard.whiteBishops = (chessBoard.whiteBishops ^ move.to) | move.from;
		break;
	case WHITEROOK:
		chessBoard.whiteRooks = (chessBoard.whiteRooks ^ move.to) | move.from;
		break;
	case WHITEQUEEN:
		chessBoard.whiteQueens = (chessBoard.whiteQueens ^ move.to) | move.from;
		break;
	case WHITEKING:
		chessBoard.whiteKing = (chessBoard.whiteKing ^ move.to) | move.from;
		break;
	case BLACKPAWN:
		chessBoard.blackPawns = (chessBoard.blackPawns ^ move.to) | move.from;
		if (move.promotionPiece) {
			unPromote(chessBoard, move);
		}
		break;
	case BLACKKNIGHT:
		chessBoard.blackKnights = (chessBoard.blackKnights ^ move.to) | move.from;
		break;
	case BLACKBISHOP:
		chessBoard.blackBishops = (chessBoard.blackBishops ^ move.to) | move.from;
		break;
	case BLACKROOK:
		chessBoard.blackRooks = (chessBoard.blackRooks ^ move.to) | move.from;
		break;
	case BLACKQUEEN:
		chessBoard.blackQueens = (chessBoard.blackQueens ^ move.to) | move.from;
		break;
	case BLACKKING:
		chessBoard.blackKing = (chessBoard.blackKing ^ move.to) | move.from;
		break;
	}

	switch (othertype) // if there is a captured piece, place it back on the board
	{
	case WHITEPAWN:
		chessBoard.whitePawns |= move.to;
		break;
	case WHITEKNIGHT:
		chessBoard.whiteKnights |= move.to;
		break;
	case WHITEBISHOP:
		chessBoard.whiteBishops |= move.to;
		break;
	case WHITEROOK:
		chessBoard.whiteRooks |= move.to;
		break;
	case WHITEQUEEN:
		chessBoard.whiteQueens |= move.to;
		break;
	case WHITEKING:
		chessBoard.whiteKing |= move.to;
		break;
	case BLACKPAWN:
		chessBoard.blackPawns |= move.to;
		break;
	case BLACKKNIGHT:
		chessBoard.blackKnights |= move.to;
		break;
	case BLACKBISHOP:
		chessBoard.blackBishops |= move.to;
		break;
	case BLACKROOK:
		chessBoard.blackRooks |= move.to;
		break;
	case BLACKQUEEN:
		chessBoard.blackQueens |= move.to;
		break;
	case BLACKKING:
		chessBoard.blackKing |= move.to;
		break;
	}
	updateChessBoardPieces(chessBoard);
}

/// <summary>
/// checks validity of current player position, assuming they just moved, chessBoard color should be the player next to move
/// </summary>
/// <param name="chessBoard">the chessBoard that position is checked</param>
/// <returns>true if move is valid, false if not</returns>
bool isValidMove(ChessBoard& chessBoard) {
	///A chess move is valid if the 8 directions away from king do not contain a queen, and respective rook or bishop
	///because should not matter if the king is already in check or not because the movev would have to solve these issues to be valid ???
	std::uint64_t& king{ chessBoard.whiteToMove ? chessBoard.blackKing : chessBoard.whiteKing };
	std::uint64_t& otherQueens{ chessBoard.whiteToMove ? chessBoard.whiteQueens : chessBoard.blackQueens };
	std::uint64_t& otherRooks{ chessBoard.whiteToMove ? chessBoard.whiteRooks : chessBoard.blackRooks };
	std::uint64_t& otherBishops{ chessBoard.whiteToMove ? chessBoard.whiteBishops : chessBoard.blackBishops };
	std::uint64_t& currPieces{ chessBoard.whiteToMove ? chessBoard.blackPieces : chessBoard.whitePieces };

	bool notInvalid{ true };
	int shift{ 8 };

	while (notInvalid) { // up
		std::uint64_t thisSquare{ king << shift };
		if (thisSquare & otherQueens || thisSquare & otherRooks) return false; // if the king can be seen by a rook or a queen, move is not valid
		notInvalid = (thisSquare) && !(thisSquare & currPieces); // notInvalid if on board, and another piece of the same color is not between
		shift += 8;
	}

	notInvalid = true;
	shift = 8;

	while (notInvalid) { // down
		std::uint64_t thisSquare{ king >> shift };
		if (thisSquare & otherQueens || thisSquare & otherRooks) return false; // if the king can be seen by a rook or a queen, move is not valid
		notInvalid = (thisSquare) && !(thisSquare & currPieces); // notInvalid if on board, and another piece of the same color is not between
		shift += 8;
	}

	notInvalid = true;
	shift = 1;

	while (notInvalid) { // right
		std::uint64_t thisSquare{ (king & NOTRIGHTEDGE) << shift };
		if (thisSquare & otherQueens || thisSquare & otherRooks) return false; // if the king can be seen by a rook or a queen, move is not valid
		notInvalid = (thisSquare & NOTRIGHTEDGE) && !(thisSquare & currPieces); // notInvalid if on board, and another piece of the same color is not between
		++shift;
	}

	notInvalid = true;
	shift = 1;

	while (notInvalid) { // left
		std::uint64_t thisSquare{ (king & NOTLEFTEDGE) >> shift }; // no moves if already on the edge
		if (thisSquare & otherQueens || thisSquare & otherRooks) return false; // if the king can be seen by a rook or a queen, move is not valid
		notInvalid = (thisSquare & NOTLEFTEDGE) && !(thisSquare & currPieces); // notInvalid if on board, and another piece of the same color is not between
		++shift;
	}

	notInvalid = true;
	shift = 7;

	while (notInvalid) { // down left
		std::uint64_t thisSquare{ (king & NOTRIGHTEDGE) >> shift };
		if (thisSquare & otherQueens || thisSquare & otherBishops) return false; // if the king can be seen by a bishop or a queen, move is not valid
		notInvalid = (thisSquare & NOTRIGHTEDGE) && !(thisSquare & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
		shift += 7;
	}

	notInvalid = true;
	shift = 9;

	while (notInvalid) { // up right
		std::uint64_t thisSquare{ (king & NOTRIGHTEDGE) << shift };
		if (thisSquare & otherQueens || thisSquare & otherBishops) return false; // if the king can be seen by a bishop or a queen, move is not valid
		notInvalid = (thisSquare & NOTRIGHTEDGE) && !(thisSquare & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
		shift += 9;
	}

	notInvalid = true;
	shift = 7;

	while (notInvalid) { // up left
		std::uint64_t thisSquare{ (king & NOTLEFTEDGE) << shift };
		if (thisSquare & otherQueens || thisSquare & otherBishops) return false; // if the king can be seen by a bishop or a queen, move is not valid
		notInvalid = (thisSquare & NOTLEFTEDGE) && !(thisSquare & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
		shift += 7;
	}

	notInvalid = true;
	shift = 9;

	while (notInvalid) { // down right
		std::uint64_t thisSquare{ (king & NOTLEFTEDGE) >> shift };
		if (thisSquare & otherQueens || thisSquare & otherBishops) return false; // if the king can be seen by a bishop or a queen, move is not valid
		notInvalid = (thisSquare & NOTLEFTEDGE) && !(thisSquare & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
		shift += 9;
	}

	if (getPawnMoves(chessBoard) & king) return false;
	if (getKnightMoves(chessBoard) & king) return false;


	return true;
}

/// <summary>
/// gets the positional value of a chess board position for all pawns of both color,
/// uses the current move color to determine if total value is positive or negative
/// </summary>
/// <param name="chessBoard">The chessboard that positional value is calculated</param>
/// <returns>int representation of current board value for pawns</returns>
int getPawnValue(const ChessBoard& chessBoard) {
	int value{ 0 };
	{
		std::uint64_t whitePawns{ chessBoard.whitePawns };
		std::uint64_t whiteBitMask{ 0 };

		value += std::popcount(whitePawns) * PAWN_VALUE;

		whiteBitMask = (whitePawns << 9) & NOTLEFTEDGE;
		whiteBitMask |= (whitePawns << 7) & NOTRIGHTEDGE;

		value += std::popcount(whitePawns & whiteBitMask) * PAWN_DEFEND_PAWN_VALUE;
		value += std::popcount(whiteBitMask & ((chessBoard.whitePieces ^ whitePawns) ^ chessBoard.whiteKing)) * PAWN_DEFEND_PIECE_VALUE;
		value += std::popcount(whiteBitMask & chessBoard.blackPawns) * PAWN_ATTACK_PAWN_VALUE;
		value += std::popcount(whiteBitMask & (chessBoard.blackPieces ^ chessBoard.blackPawns)) * PAWN_ATTACK_PIECE_VALUE;

		value += std::popcount(whitePawns & PAWN_CENTER_CONTROL_SQUARES) * PAWN_CENTER_CONTROL_VALUE;

		whitePawns >>= 16;
		while (whitePawns) {
			value += std::popcount(whitePawns) * PAWN_EXTRA_RANK_VALUE;
			whitePawns >>= 8;
		}
	}
	{
		std::uint64_t blackPawns{ chessBoard.blackPawns };
		std::uint64_t blackBitMask{ 0 };

		value -= std::popcount(blackPawns) * PAWN_VALUE;

		blackBitMask = (blackPawns >> 9) & NOTRIGHTEDGE;
		blackBitMask |= (blackPawns >> 7) & NOTLEFTEDGE;

		value -= std::popcount(blackPawns & blackBitMask) * PAWN_DEFEND_PAWN_VALUE;
		value -= std::popcount(blackBitMask & ((chessBoard.blackPieces ^ blackPawns) ^ chessBoard.blackKing)) * PAWN_DEFEND_PIECE_VALUE;
		value -= std::popcount(blackBitMask & chessBoard.whitePawns) * PAWN_ATTACK_PAWN_VALUE;
		value -= std::popcount(blackBitMask & (chessBoard.whitePieces ^ chessBoard.whitePawns)) * PAWN_ATTACK_PIECE_VALUE;
		value -= std::popcount(blackPawns & PAWN_CENTER_CONTROL_SQUARES) * PAWN_CENTER_CONTROL_VALUE;

		blackPawns <<= 16;
		while (blackPawns) {
			value -= std::popcount(blackPawns) * PAWN_EXTRA_RANK_VALUE;
			blackPawns <<= 8;
		}
	}

	return chessBoard.whiteToMove ? value : -value;
}

/// <summary>
/// gets the positional value of a chess board position for all knights of both colors,
/// uses the current move color to determine if the total value is positive or negative
/// </summary>
/// <param name="chessBoard">the chess board that the positional value is calculated</param>
/// <returns>int representation of current board value for knights</returns>
int getKnightValue(const ChessBoard& chessBoard) {
	int value{ 0 };
	{
		std::uint64_t whiteKnights{ chessBoard.whiteKnights };

		value += std::popcount(whiteKnights) * KNIGHT_VALUE;

		std::uint64_t knightMask{ 0 };
		knightMask = (whiteKnights & NOT2LEFTEDGE) << 6;
		knightMask |= (whiteKnights & NOT2RIGHTEDGE) >> 6;
		knightMask |= (whiteKnights & NOT2RIGHTEDGE) << 10;
		knightMask |= (whiteKnights & NOT2LEFTEDGE) >> 10;
		knightMask |= (whiteKnights & NOTLEFTEDGE) << 15;
		knightMask |= (whiteKnights & NOTRIGHTEDGE) >> 15;
		knightMask |= (whiteKnights & NOTRIGHTEDGE) << 17;
		knightMask |= (whiteKnights & NOTLEFTEDGE) >> 17;

		value += std::popcount(knightMask & chessBoard.whitePawns) * KNIGHT_DEFEND_PAWN_VALUE;
		value += std::popcount(((((knightMask & chessBoard.whitePieces) | chessBoard.whitePawns) ^ chessBoard.whitePawns) | chessBoard.whiteKing) ^ chessBoard.whiteKing) * KNIGHT_DEFEND_PIECE_VALUE;
		value += std::popcount(knightMask & chessBoard.blackPawns) * KNIGHT_ATTACK_PAWN_VALUE;
		value += std::popcount(((knightMask & chessBoard.blackPieces) | chessBoard.blackPawns) ^ chessBoard.blackPawns) * KNIGHT_ATTACK_PIECE_VALUE;

		value += std::popcount(whiteKnights & KNIGHT_CENTRALIZED_1) * KNIGHT_CENTRALIZED_VALUE * 1;
		value += std::popcount(whiteKnights & KNIGHT_CENTRALIZED_2) * KNIGHT_CENTRALIZED_VALUE * 2;
		value += std::popcount(whiteKnights & KNIGHT_CENTRALIZED_3) * KNIGHT_CENTRALIZED_VALUE * 3;
	}
	{
		std::uint64_t blackKnights{ chessBoard.blackKnights };

		value -= std::popcount(blackKnights) * KNIGHT_VALUE;

		std::uint64_t knightMask{ 0 };
		knightMask = (blackKnights & NOT2LEFTEDGE) << 6;
		knightMask |= (blackKnights & NOT2RIGHTEDGE) >> 6;
		knightMask |= (blackKnights & NOT2RIGHTEDGE) << 10;
		knightMask |= (blackKnights & NOT2LEFTEDGE) >> 10;
		knightMask |= (blackKnights & NOTLEFTEDGE) << 15;
		knightMask |= (blackKnights & NOTRIGHTEDGE) >> 15;
		knightMask |= (blackKnights & NOTRIGHTEDGE) << 17;
		knightMask |= (blackKnights & NOTLEFTEDGE) >> 17;

		value -= std::popcount(knightMask & chessBoard.blackPawns) * KNIGHT_DEFEND_PAWN_VALUE;
		value -= std::popcount(((((knightMask & chessBoard.blackPieces) | chessBoard.blackPawns) ^ chessBoard.blackPawns) | chessBoard.blackKing) ^ chessBoard.blackKing) * KNIGHT_DEFEND_PIECE_VALUE;
		value -= std::popcount(knightMask & chessBoard.whitePawns) * KNIGHT_ATTACK_PAWN_VALUE;
		value -= std::popcount(((knightMask & chessBoard.whitePawns) | chessBoard.whitePawns) ^ chessBoard.whitePawns) * KNIGHT_ATTACK_PIECE_VALUE;

		value -= std::popcount(blackKnights & KNIGHT_CENTRALIZED_1) * KNIGHT_CENTRALIZED_VALUE * 1;
		value -= std::popcount(blackKnights & KNIGHT_CENTRALIZED_2) * KNIGHT_CENTRALIZED_VALUE * 2;
		value -= std::popcount(blackKnights & KNIGHT_CENTRALIZED_3) * KNIGHT_CENTRALIZED_VALUE * 3;
	}
	return chessBoard.whiteToMove ? value : -value;
}

/// <summary>
/// gets the value of one color of rooks on the board
/// </summary>
/// <param name="chessBoard">the chessboard that whiteToMove value is gotten from</param>
/// <returns>value of one rook</returns>
int getOneRookValue(const ChessBoard& chessBoard, bool whiteToMove) {
	int value{ 0 };
	int emptyCount{ 0 };
	int otherPawnCount{ 0 };
	int otherPieceCount{ 0 };
	int thisPawnCount{ 0 };
	int thisPieceCount{ 0 };

	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	std::uint64_t otherPieces = whiteToMove ? chessBoard.blackPieces : chessBoard.whitePieces;
	std::uint64_t currRooks = whiteToMove ? chessBoard.whiteRooks : chessBoard.blackRooks;
	std::uint64_t thisMove{ 0 };

	std::uint64_t myCurrPawns = whiteToMove ? chessBoard.whitePawns : chessBoard.blackPawns;
	std::uint64_t myCurrPieces = currPieces ^ myCurrPawns;
	std::uint64_t myOtherPawns = whiteToMove ? chessBoard.blackPawns : chessBoard.whitePawns;
	std::uint64_t myOtherPieces = otherPieces ^ myOtherPawns;

	while (currRooks) {
		int thisRook = std::countr_zero(currRooks); // get current rook
		std::uint64_t rook{ 1ULL << thisRook };
		value += ROOK_VALUE;

		bool valid{ true };
		int shift{ 1 };

		while (valid) { // left
			thisMove = (rook & NOTLEFTEDGE) >> shift;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			++shift;
			++emptyCount;
		}
		emptyCount -= ((rook & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces)) != 0;
		//emptyCount -= std::popcount((rook & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces));
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		thisPieceCount += (((thisMove & myCurrPieces) | rook) ^ rook) != 0; // and not this rook
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | rook) ^ rook); // and not this rook
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		//if (thisMove & currPieces) ++otherPawnCount; See if fasater

		valid = true;
		shift = 1;

		while (valid) { // right
			thisMove = (rook & NOTRIGHTEDGE) << shift;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			++shift;
			++emptyCount;
		}
		emptyCount -= ((rook & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (((thisMove & myCurrPieces) | rook) ^ rook) != 0; // and not this rook
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//emptyCount -= std::popcount((rook & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces));
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | rook) ^ rook); // and not this rook
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 8;

		while (valid) { // up
			thisMove = rook << shift;
			valid = (thisMove) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on board, or if capture, or if in currPieces
			shift += 8;
			++emptyCount;
		}
		//emptyCount -= std::popcount((thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces));
		--emptyCount;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (((thisMove & myCurrPieces) | rook) ^ rook) != 0; // and not this rook
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//--emptyCount;
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | rook) ^ rook); // and not this rook
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 8;

		while (valid) { // down
			thisMove = rook >> shift ;
			valid = (thisMove) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on board, or if capture, or if in currPieces
			shift += 8;
			++emptyCount;
		}
		//emptyCount -= std::popcount((thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces));
		--emptyCount;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (((thisMove & myCurrPieces) | rook) ^ rook) != 0; // and not this rook
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//--emptyCount;
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | rook) ^ rook); // and not this rook
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		currRooks ^= 1ULL << thisRook; // remove thisRook from board
	}
	return (value +
		thisPawnCount * ROOK_DEFEND_PAWN_VALUE +
		thisPieceCount * ROOK_DEFEND_PIECE_VALUE +
		otherPawnCount * ROOK_ATTACK_PAWN_VALUE +
		otherPieceCount * ROOK_ATTACK_PIECE_VALUE +
		emptyCount * ROOK_EMPTY_SQUARE_VALUE);
}

/// <summary>
/// gets the total value of all rooks on the board using
/// empty squares seen by each rook, this pawn, this piece, other pawn, other piece
/// </summary>
/// <param name="chessBoard">const in nature, the chessboard that rook value is found</param>
/// <returns>the total value of chessboard for whiteToMove</returns>
int getRookValue(const ChessBoard& chessBoard) {
	int white{ getOneRookValue(chessBoard, true) };
	int black{ getOneRookValue(chessBoard, false) };
	return white - black;
}

/// <summary>
/// gets the value of one color of bishops on the board
/// </summary>
/// <param name="chessBoard">the chessboard that whiteToMove value is gotten from</param>
/// <returns>value of one bishop</returns>
int getOneBishopValue(const ChessBoard& chessBoard, bool whiteToMove) {
	int value{ 0 };
	int emptyCount{ 0 };
	int otherPawnCount{ 0 };
	int otherPieceCount{ 0 };
	int thisPawnCount{ 0 };
	int thisPieceCount{ 0 };

	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	std::uint64_t otherPieces = whiteToMove ? chessBoard.blackPieces : chessBoard.whitePieces;
	std::uint64_t currBishops = whiteToMove ? chessBoard.whiteBishops : chessBoard.blackBishops;
	std::uint64_t thisMove{ 0 };

	std::uint64_t myCurrPawns = whiteToMove ? chessBoard.whitePawns : chessBoard.blackPawns;
	std::uint64_t myCurrPieces = currPieces ^ myCurrPawns;
	std::uint64_t myOtherPawns = whiteToMove ? chessBoard.blackPawns : chessBoard.whitePawns;
	std::uint64_t myOtherPieces = otherPieces ^ myOtherPawns;

	while (currBishops) {
		int thisBishop = std::countr_zero(currBishops); // get current bishop
		std::uint64_t bishop{ 1ULL << thisBishop };
		value += BISHOP_VALUE;

		bool valid{ true };
		int shift{ 7 };

		while (valid) { // down left
			thisMove = (bishop & NOTRIGHTEDGE) >> shift;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 7;
			++emptyCount;
		}
		emptyCount -= ((bishop & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | static_cast<uint64_t>(!thisMove)) != 0;
		//emptyCount -= static_cast<bool>(std::popcount((bishop & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | !thisMove));
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		thisPieceCount += (thisMove & myCurrPieces & ~bishop) != 0;
		//thisPieceCount += ((thisMove & myCurrPieces) | bishop) ^ bishop) != 0;
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | bishop) ^ bishop); // and not this bishop
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 9;

		while (valid) { // up right
			thisMove = (bishop & NOTRIGHTEDGE) << shift;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 9;
			++emptyCount;
		}
		emptyCount -= ((bishop & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | static_cast<uint64_t>(!thisMove)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (thisMove & myCurrPieces & ~bishop) != 0;
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;

		//emptyCount -= std::popcount((bishop & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | !thisMove);
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | bishop) ^ bishop); // and not this bishop
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 7;

		while (valid) { // up left
			thisMove = (bishop & NOTLEFTEDGE) << shift;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 7;
			++emptyCount;
		}
		emptyCount -= ((bishop & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | static_cast<uint64_t>(!thisMove)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (thisMove & myCurrPieces & ~bishop) != 0;
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;

		//emptyCount -= std::popcount((bishop & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | !thisMove);
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | bishop) ^ bishop); // and not this bishop
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 9;

		while (valid) { // down right
			thisMove = (bishop & NOTLEFTEDGE) >> shift;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 9;
			++emptyCount;
		}
		emptyCount -= ((bishop & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | static_cast<uint64_t>(!thisMove)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (thisMove & myCurrPieces & ~bishop) != 0;
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;

		//emptyCount -= std::popcount((bishop & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | !thisMove);
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | bishop) ^ bishop); // and not this bishop
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);


		currBishops ^= 1ULL << thisBishop; // remove thisBishop from board
	}
	return (value +
		thisPawnCount * BISHOP_DEFEND_PAWN_VALUE +
		thisPieceCount * BISHOP_DEFEND_PIECE_VALUE +
		otherPawnCount * BISHOP_ATTACK_PAWN_VALUE +
		otherPieceCount * BISHOP_ATTACK_PIECE_VALUE +
		emptyCount * BISHOP_EMPTY_SQUARE_VALUE);
}

/// <summary>
/// gets the total value of all bishops on the board using
/// empty squares seen by each bishop, this pawn, this piece, other pawn, other piece
/// </summary>
/// <param name="chessBoard">const in nature, the chessboard that bishop value is found</param>
/// <returns>the total value of chessboard for whiteToMove</returns>
int getBishopValue(const ChessBoard& chessBoard) {
	int white{ getOneBishopValue(chessBoard, true) };
	int black{ getOneBishopValue(chessBoard, false) };
	return white - black;
}

/// <summary>
/// gets the value of one color of queens on the board
/// </summary>
/// <param name="chessBoard">the chessboard that whiteToMove value is gotten from</param>
/// <returns>value of one queen</returns>
int getOneQueenValue(const ChessBoard& chessBoard, bool whiteToMove) {
	int value{ 0 };
	int emptyCount{ 0 };
	int otherPawnCount{ 0 };
	int otherPieceCount{ 0 };
	int thisPawnCount{ 0 };
	int thisPieceCount{ 0 };

	std::uint64_t currPieces = whiteToMove ? chessBoard.whitePieces : chessBoard.blackPieces;
	std::uint64_t otherPieces = whiteToMove ? chessBoard.blackPieces : chessBoard.whitePieces;
	std::uint64_t currQueens = whiteToMove ? chessBoard.whiteQueens : chessBoard.blackQueens;
	std::uint64_t thisMove{ 0 };

	std::uint64_t myCurrPawns = whiteToMove ? chessBoard.whitePawns : chessBoard.blackPawns;
	std::uint64_t myCurrPieces = currPieces ^ myCurrPawns;
	std::uint64_t myOtherPawns = whiteToMove ? chessBoard.blackPawns : chessBoard.whitePawns;
	std::uint64_t myOtherPieces = otherPieces ^ myOtherPawns;

	while (currQueens) {
		int thisQueen = std::countr_zero(currQueens); // get current rook
		std::uint64_t queen{ 1ULL << thisQueen };
		value += QUEEN_VALUE;

		bool valid{ true };
		int shift{ 1 };

		while (valid) { // left
			thisMove = (queen & NOTLEFTEDGE) >> shift;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			++shift;
			++emptyCount;
		}
		emptyCount -= ((queen & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (((thisMove & myCurrPieces) | queen) ^ queen) != 0; // and not this queen
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//emptyCount -= std::popcount((queen & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces));
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | queen) ^ queen); // and not this queen
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		//if (thisMove & currPieces) ++otherPawnCount; See if fasater

		valid = true;
		shift = 1;

		while (valid) { // right
			thisMove = (queen & NOTRIGHTEDGE) << shift;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			++shift;
			++emptyCount;
		}
		emptyCount -= ((queen & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (((thisMove & myCurrPieces) | queen) ^ queen) != 0; // and not this queen
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//emptyCount -= std::popcount((queen & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces));
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | queen) ^ queen); // and not this queen
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 8;

		while (valid) { // up
			thisMove = queen << shift;
			valid = (thisMove) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on board, or if capture, or if in currPieces
			shift += 8;
			++emptyCount;
		}
		--emptyCount;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (((thisMove & myCurrPieces) | queen) ^ queen) != 0; // and not this queen
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//--emptyCount;
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | queen) ^ queen); // and not this queen
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 8;

		while (valid) { // down
			thisMove = queen >> shift;
			valid = (thisMove) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on board, or if capture, or if in currPieces
			shift += 8;
			++emptyCount;
		}
		--emptyCount;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (((thisMove & myCurrPieces) | queen) ^ queen) != 0; // and not this queen
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;
		//--emptyCount;
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | queen) ^ queen); // and not this queen
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);


		valid = true;
		shift = 7;

		while (valid) { // down left
			thisMove = (queen & NOTRIGHTEDGE) >> shift;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 7;
			++emptyCount;
		}
		emptyCount -= ((queen & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | static_cast<uint64_t>(!thisMove)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (thisMove & myCurrPieces & ~queen) != 0;
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;

		//emptyCount -= std::popcount((queen & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | !thisMove);
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | queen) ^ queen); // and not this queen
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 9;

		while (valid) { // up right
			thisMove = (queen & NOTRIGHTEDGE) << shift;
			valid = (thisMove & NOTRIGHTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 9;
			++emptyCount;
		}
		emptyCount -= ((queen & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | static_cast<uint64_t>(!thisMove)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (thisMove & myCurrPieces & ~queen) != 0;
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;

		//emptyCount -= std::popcount((queen & ~NOTRIGHTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | !thisMove);
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | queen) ^ queen); // and not this queen
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 7;

		while (valid) { // up left
			thisMove = (queen & NOTLEFTEDGE) << shift;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 7;
			++emptyCount;
		}
		emptyCount -= ((queen & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | static_cast<uint64_t>(!thisMove)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (thisMove & myCurrPieces & ~queen) != 0;
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;

		//emptyCount -= std::popcount((queen & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | !thisMove);
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | queen) ^ queen); // and not this queen
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);

		valid = true;
		shift = 9;

		while (valid) { // down right
			thisMove = (queen & NOTLEFTEDGE) >> shift;
			valid = (thisMove & NOTLEFTEDGE) && !(thisMove & otherPieces) && !(thisMove & currPieces); // next move not valid if on edge, or if capture, or if in currPieces
			shift += 9;
			++emptyCount;
		}
		emptyCount -= ((queen & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | static_cast<uint64_t>(!thisMove)) != 0;
		thisPawnCount += (thisMove & myCurrPawns) != 0;
		thisPieceCount += (thisMove & myCurrPieces & ~queen) != 0;
		otherPawnCount += (thisMove & myOtherPawns) != 0;
		otherPieceCount += (thisMove & myOtherPieces) != 0;

		//emptyCount -= std::popcount((queen & ~NOTLEFTEDGE) | (thisMove & myCurrPawns) | (thisMove & myCurrPieces) | (thisMove & myOtherPawns) | (thisMove & myOtherPieces) | !thisMove);
		//thisPawnCount += std::popcount(thisMove & myCurrPawns);
		//thisPieceCount += std::popcount(((thisMove & myCurrPieces) | queen) ^ queen); // and not this queen
		//otherPawnCount += std::popcount(thisMove & myOtherPawns);
		//otherPieceCount += std::popcount(thisMove & myOtherPieces);


		currQueens ^= 1ULL << thisQueen; // remove thisRook from board
	}
	return (value +
		thisPawnCount * QUEEN_DEFEND_PAWN_VALUE +
		thisPieceCount * QUEEN_DEFEND_PIECE_VALUE +
		otherPawnCount * QUEEN_ATTACK_PAWN_VALUE +
		otherPieceCount * QUEEN_ATTACK_PIECE_VALUE +
		emptyCount * QUEEN_EMPTY_SQUARE_VALUE);
}

/// <summary>
/// gets the total value of all queens on the board using
/// empty squares seen by each queen, this pawn, this piece, other pawn, other piece
/// </summary>
/// <param name="chessBoard">const in nature, the chessboard that queen value is found</param>
/// <returns>the total value of chessboard for whiteToMove</returns>
int getQueenValue(const ChessBoard& chessBoard) {
	int white{ getOneQueenValue(chessBoard, true) };
	int black{ getOneQueenValue(chessBoard, false) };
	return white - black;
}

/// <summary>
/// gets the value of an entire gameboard
/// </summary>
/// <param name="chessBoard">the chessboard that the value of the board is taken</param>
/// <returns>the value of the board as an integer</returns>
int getGameBoardValue(const ChessBoard& chessBoard) {
	int value{ 0 };
	value += getPawnValue(chessBoard);
	value += getKnightValue(chessBoard);
	value += getBishopValue(chessBoard);
	value += getRookValue(chessBoard);
	value += getQueenValue(chessBoard);
	return chessBoard.whiteToMove ? value : -value;
}

void getPlayerMove(ChessBoard& chessBoard) {
	int x{ 0 };
	int y{ 0 };
	int xto{ 0 };
	int yto{ 0 };
	std::cin >> x;
	std::cin >> y;
	std::cin >> xto;
	std::cin >> yto;
	std::uint64_t from{ 1ULL << x };
	from <<= y * 8;
	std::uint64_t to{ 1ULL << xto };
	to <<= yto * 8;
	std::vector<Move> moves{ generateMoves(chessBoard) };

	for (const Move& move : moves) {
		if (move.from == from && move.to == to) {
			makeMove(chessBoard, move);
			return;
		}
	}
}

/// <summary>
/// recursively searches through all valid moves for current depth, and returns the best/worst value
/// depending on which color is moving
/// </summary>
/// <param name="chessBoard"></param>
/// <param name="depth"></param>
/// <returns></returns>
int recursiveSearch(ChessBoard& chessBoard, int depth, int alpha, int beta) {
	if (depth == 0) { // base case
		return getGameBoardValue(chessBoard);
	}

	std::vector<Move> myMoves{ generateMoves(chessBoard) }; // generate moves

	for (Move myMove : myMoves) {
		makeMove(chessBoard, myMove);
		if (!isValidMove(chessBoard)) {
			unMove(chessBoard, myMove);
			continue;
		}
		if (depth == 1) {
			++MOVESGENERATED;
		}

		int currentValue{ -recursiveSearch(chessBoard, depth - 1, -beta, -alpha) }; // recursive call, switch and negate alpha and beta, player changes

		unMove(chessBoard, myMove);

		if (currentValue >= beta) { // opponent has better score
			return beta;
		}
		if (currentValue > alpha) { // better move for current player
			alpha = currentValue;
		}
		
	}
	return alpha;
}

/// <summary>
/// generates the *best* move for the computer to make by calling recursiveSearch for each valid move, and picking
/// the choice with the highest value returned by recursiveSearch
/// </summary>
/// <param name="chessBoard">The chessboard that a move is generated for</param>
/// <param name="depth">the depth of the recursive search</param>
/// <returns>The *best* move for the computer</returns>
Move getComputerMove(ChessBoard& chessBoard, int depth) {
	std::vector<Move> myMoves{ generateMoves(chessBoard) }; // generate moves
	Move bestMoveFound{ makeMove(NULL, NULL) }; // variable for best move
	int alpha{ std::numeric_limits<int>::min() + 1 }; // 1 added to prevent overflow
	constexpr int beta{ std::numeric_limits<int>::max() - 1 }; // 1 subtracted to prevent overflow

	for (Move myMove : myMoves) { // iterate through all possible moves
		makeMove(chessBoard, myMove);
		if (!isValidMove(chessBoard)) { // make sure move is valid
			unMove(chessBoard, myMove);
			continue;
		}

		std::cout << chessBoardToString(chessBoard) << "\n";

		int positionValue{ -recursiveSearch(chessBoard, depth - 1, -beta, -alpha) }; // find the *actual* value of this move
		unMove(chessBoard, myMove);
		std::cout << positionValue << "\n";

		if (positionValue > alpha) { // better move for current player
			alpha = positionValue;
			bestMoveFound = myMove;
		}

		/*if (myMove.to == 268435456) {
			std::cout << "debug value == " << positionValue << "\n";
			std::cout << chessBoardToString(chessBoard);
			
		}
		if (myMove.to == 134217728) {
			std::cout << "debug value == " << positionValue << "\n";
			std::cout << chessBoardToString(chessBoard);
		}
		if (myMove.to == 1048576) {
			std::cout << "debug value == " << positionValue << "\n";
			std::cout << chessBoardToString(chessBoard);
		}*/
	}
	return bestMoveFound; // return best move
}

Move getComputerMove(ChessBoard& chessBoard) {
	//get all current moves
	//get all other moves for current moves
	// from those moves, pick the move with the highest, lowest evaluation
	std::vector<Move> myMoves{ generateMoves(chessBoard) };
	Move currentMove{ makeMove(0ULL, 0ULL)};
	int myBestMove{ std::numeric_limits<int>::min() };
	for (Move myMove : myMoves) {
		makeMove(chessBoard, myMove);
		if (!isValidMove(chessBoard)) {
			unMove(chessBoard, myMove);
			continue;
		}
		std::vector<Move> otherMoves{ generateMoves(chessBoard) };
		int bestMove{ std::numeric_limits<int>::min() };
		for (Move otherMove : otherMoves) {
			makeMove(chessBoard, otherMove);
			if (!isValidMove(chessBoard)) {
				unMove(chessBoard, otherMove);
				continue;
			}
			int moveValue{ -getGameBoardValue(chessBoard) };
			if (bestMove < moveValue) {
				bestMove = moveValue;
			}
			unMove(chessBoard, otherMove);
		}
		if (myBestMove < -bestMove) {
			currentMove = myMove;
			myBestMove = -bestMove;
		}
		unMove(chessBoard, myMove);
	}
	return currentMove;
}

int main() {

	ChessBoard chessBoard;

	makeNewChessBoard(chessBoard);

	updateChessBoardPieces(chessBoard);

	Move computerMove{ getComputerMove(chessBoard, 5) };
	makeMove(chessBoard, computerMove);
	std::cout << chessBoardToString(chessBoard);

	std::cout << getGameBoardValue(chessBoard) << "\n";

	std::cout << MOVESGENERATED << " Moves generated\n";

	while (true)
	{
		std::cout << "Enter to continue";
		std::string bad;
		std::cin >> bad;
		computerMove = getComputerMove(chessBoard, 5);
		makeMove(chessBoard, computerMove);
		std::cout << chessBoardToString(chessBoard);

		std::cout << getGameBoardValue(chessBoard) << "\n";
		std::cout << MOVESGENERATED << " Moves generated\n";
	}

	/*while (true) {

		std::cout << chessBoardToString(chessBoard);

		std::cout << getGameBoardValue(chessBoard);

		getPlayerMove(chessBoard);
	}

	std::cout << chessBoardToString(chessBoard);

	std::cout << getGameBoardValue(chessBoard);*/

	//ChessBoard chessBoard;
	//chessBoard.whiteKnights = 1ULL << 35;
	//chessBoard.whiteBishops = 1ULL << 56;
	//chessBoard.whiteKnights = 1ULL << 9;
	//chessBoard.blackRooks = 1ULL << 1;
	//!thisMove, and bishop & ~NOTRIGHTEDGE need to be seperate, only happen once
	//chessBoard.whiteBishops = 1ULL << 28;
	//chessBoard.blackRooks = 1ULL << 21;
	//chessBoard.whiteBishops += 1ULL << 19;
	//chessBoard.blackBishops += 1ULL << (35 + 8 + 8 + 8);

	//chessBoard.whiteRooks = 1ULL;
	//updateChessBoardPieces(chessBoard);

	//std::cout << chessBoardToString(chessBoard) << "\n";


	//std::cout << getOneRookValue(chessBoard) << "\n";
	//std::cout << getBishopValue(chessBoard) << "\n";

    /*std::vector<Move> validMoves{ generateMoves(chessBoard) };

	for (int i = 0; i < validMoves.size(); ++i) {
		std::cout << validMoves.at(i).from << ", " << validMoves.at(i).to << ", " << validMoves.at(i).capturedPiece << ", " << validMoves.at(i).promotionPiece << ", " << static_cast<int>(validMoves.at(i).specialMoves) << "\n";
	}
	
	for (int i = 0; i < validMoves.size(); ++i) {
		makeMove(chessBoard, validMoves[i]);
		std::cout << chessBoardToString(chessBoard) << "\n";
		unMove(chessBoard, validMoves[i]);
		std::cout << chessBoardToString(chessBoard) << "\n\n\n";
	}*/

	//problem seems to be move having multiple possible moves in it, move.to in generate moves??? pawn ppush first does 2, knights first do 2

	//chessBoard.whiteKnights = 0b00000000'00000000'00000001'00000000'00000000'00000000'00000000'00000000;
	////chessBoard.whitePawns = chessBoard.whiteKnights << 6;
	////chessBoard.blackRooks = 0b00000000'00000000'00000000'00000000'00001000'00000000'00000000'00010000;
	//updateChessBoardPieces(chessBoard);
	//std::uint64_t validMoves{ getKnightMoves(chessBoard) };
	//chessBoard.whiteKnights |= validMoves;
	//chessBoard.blackRooks = 0;
	//updateChessBoardPieces(chessBoard);
	//std::cout << chessBoardToString(chessBoard) << "\n";

	///*std::uint64_t validMoves2{ getQueenMoves(chessBoard, true) };
	//chessBoard.whiteQueens = validMoves2;
	//updateChessBoardPieces(chessBoard);
	//std::cout << chessBoardToString(chessBoard) << "\n";*/
	return 0;
}
