#ifndef UTILS_H
#define UTILS_H

#include "pch.h"
#include "BitboardConstants.hpp"
#include "CustomTypes.hpp"

namespace Utils {
    // Helper to count the number of set bits in a bitboard
    static inline int countSetBits(const uint64_t& bitboard) {
        #if defined(_MSC_VER) // MSVC
            return static_cast<int>(__popcnt64(bitboard));
        #else // GCC and Clang
            return static_cast<int>(__builtin_popcountll(bitboard));
        #endif
    }


    // Helper function to find the index of first set bit and last set bit
    // Use inline to avoid function call overhead
    static inline int findFirstSetBit(uint64_t value) {
        #if defined(_MSC_VER) // MSVC
                unsigned long index;
                if (_BitScanForward64(&index, value)) {
                    return static_cast<int>(index);
                }
                return -1; // No bits are set
        #else // GCC and Clang
                return value ? __builtin_ctzll(value) : -1;
        #endif
    }

    static inline int getFile(int square) {
        return square & 7;
    }

    static inline int getRank(int square) {
        return square >> 3;
    }

    static inline int getSquare(uint64_t rank, uint64_t file) {
        return static_cast<int>(8 * rank + file);
    }

    static inline int getRow(int square, bool white) {
        return white ? 7 - (square / 8) : square / 8;
    }

    static inline int getCol(int square, bool white) {
        return white ? square % 8 : 7 - (square % 8);
    }

    static inline void popBit(uint64_t& bn, int sq) { bn &= ~(1ULL << sq); }

    static inline int bitScanForward(uint64_t bb) {
        static const int index64[64] = {
            0, 47, 1, 56, 48, 27, 2, 60,
            57, 49, 41, 37, 28, 16, 3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11, 4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30, 9, 24,
            13, 18, 8, 12, 7, 6, 5, 63 };

        static const uint64_t debruijn64 = 0x03f79d71b4cb0a89;
        return (int)index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
    }

    static inline uint64_t setOccupancy(int index, int bits_in_mask, uint64_t attack_mask) {
        uint64_t occupancy = 0ULL;
        for (int bit = 0; bit < bits_in_mask; bit++)
        {
            int lsb_sq = bitScanForward(attack_mask);
            popBit(attack_mask, lsb_sq);
            if (index & (1 << bit))
            {
                occupancy |= 1ULL << lsb_sq;
            }
        }
        return occupancy;
    }

    inline int get_direction(int diff) {
        if (diff % 8 == 0) return (diff > 0) ? 8 : -8;  // Vertical
        if (diff % 7 == 0) return (diff > 0) ? 7 : -7;  // Diagonal
        if (diff % 9 == 0) return (diff > 0) ? 9 : -9;  // Diagonal
        if (diff % 1 == 0) return (diff > 0) ? 1 : -1;  // Horizontal

        return 0;  // Invalid (should not happen if called correctly)
    }

    inline int calculateDistance(int sq1, int sq2) {
        int dx = abs((sq1 % 8) - (sq2 % 8));
        int dy = abs((sq1 / 8) - (sq2 / 8));
        return std::max(dx, dy); // Chebyshev distance (also known as chessboard distance)
    }
}

#endif