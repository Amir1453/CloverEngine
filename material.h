#pragma once

int psqt[13][2][64];

int mat[2][7] = {
    {0, 72, 302, 331, 488, 1094, 0},
    {0, 117, 371, 409, 708, 1262, 0},
};

int bonusTable[7][2][64] = {
    {},
    {
        {
            42, 1, 4, 5, 1, 0, 3, 1,
            146, 74, 116, 138, 132, 75, 29, 0,
            6, 14, 39, 53, 64, 81, 44, 9,
            2, 1, 0, 16, 26, 25, 7, -4,
            -2, -7, 3, 7, 15, 20, 11, -11,
            -11, -13, -9, -4, 6, 14, 15, -11,
            -6, -1, -5, -1, -1, 24, 26, 0,
            31, 17, -4, 6, 4, 1, -2, 1,

        },
        {
            -1, -3, 2, 4, -1, 1, 0, 0,
            124, 143, 114, 82, 70, 97, 118, 129,
            76, 67, 40, 8, 3, 17, 42, 54,
            37, 28, 17, -11, -11, 9, 23, 25,
            23, 23, 5, -1, -6, 9, 18, 16,
            17, 17, 14, 14, 17, 13, 11, 11,
            26, 20, 21, 21, 33, 14, 16, 18,
            15, 4, 0, 0, 2, 1, 1, 2,

        },
    },
    {
        {
            -163, -55, -49, -69, 3, -85, -54, -108,
            -6, -3, 30, 50, 19, 86, 1, -1,
            -23, 3, 15, 29, 65, 68, 32, 7,
            2, 16, 31, 34, 30, 49, 30, 37,
            -3, 9, 20, 29, 32, 26, 36, 22,
            -12, 3, 3, 11, 18, 16, 17, 8,
            -18, -5, -9, 3, 4, 2, 7, 7,
            -28, -13, -29, -6, 0, -4, -7, -17,

        },
        {
            -15, 4, 21, 24, 7, -11, -17, -60,
            3, 19, 11, 23, 26, -16, 10, -11,
            -6, 5, 41, 34, 15, 22, -1, -12,
            5, 18, 41, 52, 56, 40, 30, 4,
            3, 15, 43, 45, 44, 46, 20, 16,
            -8, 1, 12, 34, 30, 10, 9, -3,
            4, 8, -4, 7, 11, -4, 13, 23,
            2, -15, 6, 8, 5, 1, -1, -2,

        },
    },
    {
        {
            -56, -71, -38, -99, -78, -58, -41, -83,
            -19, -6, -9, -3, -4, -22, -40, -17,
            -25, -14, -2, 3, 4, 17, 0, -9,
            -30, 9, 3, 27, 15, 13, 14, -16,
            -9, 5, 9, 14, 25, 8, 11, 13,
            5, 15, 1, 13, 13, 10, 19, 20,
            19, 10, 15, 3, 2, 12, 22, 26,
            17, 23, -6, -6, -8, -5, 13, 25,

        },
        {
            7, 41, 27, 38, 33, 17, 12, 5,
            12, 9, 24, 22, 17, 25, 8, 8,
            7, 18, 4, 11, 15, 7, 21, 3,
            9, 13, 13, 23, 24, 18, 18, 17,
            -4, 4, 19, 18, 20, 20, 10, -17,
            -3, 8, 0, 15, 15, -2, 2, 9,
            1, -16, -4, 5, 7, -7, -11, -14,
            -9, -3, 1, -2, 2, 6, 4, -13,

        },
    },
    {
        {
            29, 4, 16, -1, -1, 8, 31, 44,
            2, -10, 10, 23, 13, 19, 19, 59,
            -17, 14, 6, 20, 33, 42, 71, 36,
            -10, 1, 14, 30, 25, 20, 29, 10,
            -23, -22, -17, -6, -6, -12, 13, -1,
            -31, -19, -26, -18, -15, -14, 20, -2,
            -40, -23, -17, -14, -12, -5, 11, -16,
            -19, -18, -10, -6, -5, -6, 4, -3,

        },
        {
            38, 50, 44, 48, 46, 45, 39, 39,
            38, 46, 44, 47, 49, 31, 28, 11,
            37, 29, 39, 32, 22, 28, 4, 14,
            30, 33, 33, 30, 28, 26, 16, 23,
            17, 28, 29, 21, 20, 24, 11, 7,
            -2, 5, 4, -2, -5, -5, -16, -21,
            -13, -11, -10, -15, -15, -22, -26, -15,
            -6, -5, -5, -12, -11, -4, -15, -22,

        },
    },
    {
        {
            9, 0, 14, 31, 13, 36, 28, 9,
            5, -44, -21, -14, -25, -19, -16, 44,
            -7, -1, -14, -2, -8, 30, 16, 32,
            -6, 5, -7, -2, -3, 14, 23, 14,
            6, 5, 6, 3, 7, 15, 22, 20,
            3, 19, 14, 5, 8, 13, 25, 26,
            7, 12, 19, 15, 17, 23, 29, 46,
            14, 14, 16, 26, 27, 0, 19, 15,

        },
        {
            32, 50, 58, 56, 83, 61, 60, 51,
            34, 74, 96, 91, 135, 109, 104, 65,
            30, 41, 73, 80, 112, 121, 107, 60,
            54, 60, 72, 89, 117, 100, 116, 92,
            37, 70, 52, 81, 74, 75, 68, 72,
            27, 19, 38, 28, 32, 47, 26, 15,
            9, 3, -19, 6, 3, -24, -19, -32,
            -5, -2, 1, 6, -9, 3, 20, 24,

        },
    },
    {
        {
            -52, 88, 90, 5, 8, -59, 45, -43,
            35, 61, 49, 39, -20, 27, 4, -51,
            -72, 18, 13, -48, -28, -9, 9, -45,
            -89, -26, -75, -105, -69, -72, -85, -109,
            -52, -77, -44, -81, -77, -74, -80, -124,
            -5, -23, -45, -60, -49, -36, -11, -22,
            30, -11, -24, -69, -52, -52, 9, 26,
            28, 40, 13, -44, -8, -35, 37, 41,

        },
        {
            -209, -86, -45, -16, 9, 7, -37, -130,
            -14, 33, 51, 34, 52, 52, 63, 2,
            15, 55, 58, 62, 56, 63, 60, 10,
            15, 43, 65, 73, 66, 66, 59, 16,
            -8, 32, 48, 65, 64, 53, 42, 6,
            -30, 3, 25, 43, 38, 24, -2, -25,
            -49, -15, 3, 13, 12, 9, -24, -56,
            -88, -58, -37, -32, -63, -26, -58, -111,

        },
    },
};


void initPSQT() {
  for(int p = PAWN; p <= KING; p++) {
    for(int s = MG; s <= EG; s++) {
      for(int sq = 0; sq < 64; sq++) {
        psqt[getType(p, WHITE)][s][sq] = +mat[s][p] + bonusTable[p][s][mirror(BLACK, sq)];
        psqt[getType(p, BLACK)][s][sq] = -mat[s][p] - bonusTable[p][s][mirror(WHITE, sq)];
      }
    }
  }
}

void addPiece(int16_t score[], int p, int sq) {
  score[MG] += psqt[p][MG][sq];
  score[EG] += psqt[p][EG][sq];
}

void remPiece(int16_t score[], int p, int sq) {
  score[MG] -= psqt[p][MG][sq];
  score[EG] -= psqt[p][EG][sq];
}
