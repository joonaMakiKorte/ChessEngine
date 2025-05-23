#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include <cstdint>

// Sides are assigned an enum
enum Color : uint8_t {
    BLACK = 0,
    WHITE = 1
};

// Each piece is assigned a unique integer (4 bits)
enum PieceType : uint8_t {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    EMPTY = 6   // No piece
};

// Defines the type of move (4 bits)
enum MoveType : uint8_t {
    NORMAL = 0,        // Standard move
    CAPTURE = 1,       // Capturing a piece
    CASTLING = 2,      // Castling (O-O, O-O-O)
    EN_PASSANT = 3,    // En passant capture
    PROMOTION = 4,     // Pawn promotion
    PROMOTION_CAPTURE = 5,  // Pawn promotion with capture
    PAWN_DOUBLE_PUSH = 6
};

// Direction type (8 possible directions + 0 for no direction)
enum Direction : int8_t {
    NORTH = 8,
    SOUTH = -8,
    EAST = 1,
    WEST = -1,
    NORTH_EAST = 9,
    NORTH_WEST = 7,
    SOUTH_EAST = -7,
    SOUTH_WEST = -9,
    NONE = 0
};

// Enum for TT entry flags (bound type)
// Using uint8_t to save space
enum TTFlag : uint8_t {
    FLAG_NONE = 0,       // Represents an unused or invalid entry
    FLAG_EXACT = 1,      // Score is exact (alpha < score < beta)
    FLAG_LOWERBOUND = 2, // Score is a lower bound (score >= beta, fail-high)
    FLAG_UPPERBOUND = 3  // Score is an upper bound (score <= alpha, fail-low)
};

// The structure for each entry in the Transposition Table
struct TTEntry {
    uint64_t zobrist_key_verify = 0; // Store the full Zobrist key for verification
    uint32_t best_move = NULL_MOVE_32;    // Best move found for this position
    int16_t score = 0;             // Evaluation score (adjust type based on score range)
    int8_t depth = -1;             // Depth searched (-1 indicates unused/invalid)
    TTFlag flag = FLAG_NONE;       // Flag indicating score type (EXACT, LOWER, UPPER)

    // Ensure struct is packed if necessary, though alignment might make it naturally packed.
    // Check sizeof(TTEntry) to be sure. Let's assume 8+4+2+1+1 = 16 bytes (+ potential padding)
};

// Board state is stored as a bitmask
struct BoardState {
    uint8_t flags = 0; // 8-bit bitfield to store state flags

    static constexpr uint8_t CHECK_WHITE = 1 << 0; // 0000 0001
    static constexpr uint8_t CHECK_BLACK = 1 << 1; // 0000 0010
    static constexpr uint8_t STALEMATE = 1 << 2; // 0000 0100
    static constexpr uint8_t CHECKMATE_WHITE = 1 << 3; // 0000 1000
    static constexpr uint8_t CHECKMATE_BLACK = 1 << 4; // 0001 0000
    static constexpr uint8_t DRAW_REPETITION = 1 << 5; // 0010 0000
    static constexpr uint8_t DRAW_50 = 1 << 6; // 0100 0000
    static constexpr uint8_t DRAW_INSUFFICIENT = 1 << 7; // 1000 0000

    constexpr bool isCheckWhite() const noexcept {
        return flags & CHECK_WHITE;
    }

    constexpr bool isCheckBlack() const noexcept {
        return flags & CHECK_BLACK;
    }

    constexpr bool isCheckmateWhite() const noexcept {
        return flags & CHECKMATE_WHITE;
    }

    constexpr bool isCheckmateBlack() const noexcept {
        return flags & CHECKMATE_BLACK;
    }

    constexpr bool isStalemate() const noexcept {
        return flags & STALEMATE;
    }

    constexpr bool isDrawRepetition() const noexcept {
        return flags & DRAW_REPETITION;
    }

    constexpr bool isDraw50() const noexcept {
        return flags & DRAW_50;
    }

    constexpr bool isDrawInsufficient() const noexcept {
        return flags & DRAW_INSUFFICIENT;
    }

    constexpr bool isDraw() const noexcept {
        return flags & (STALEMATE | DRAW_REPETITION | DRAW_50 | DRAW_INSUFFICIENT);
    }
};

// Save previous board states for faster state recovery in move undoing
struct UndoInfo {
    // Save castling and en passant
    uint8_t castling_rights;
    int8_t en_passant_target;

    // Flags of the game state
    uint8_t flags;

    // Board evaluation scores
    int material_delta;
    int positional_delta;
    int game_phase_delta;

    // Halfmove counter
    int half_moves;
};

// Pinned piece data
struct PinData {
    uint64_t pinned;       // All pinned pieces
    uint64_t pin_rays[64]; // Store pin ray for each pinned square
};

// Attack data, store enemy attacks squares and attacker ray
struct AttackData {
    uint64_t attack_squares;
    uint64_t attack_ray;
};

// Each direction king can get attacked from
struct KingDanger {
    uint64_t orthogonal;
    uint64_t diagonal;
    uint64_t knight;
    uint64_t pawn;
};

#endif