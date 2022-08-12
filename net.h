/*
  Clover is a UCI chess playing engine authored by Luca Metehau.
  <https://github.com/lucametehau/CloverEngine>

  Clover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Clover is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include "incbin.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <random>
#include <immintrin.h>

INCBIN(Net, EVALFILE);

const int INPUT_NEURONS = 1536;
const int SIDE_NEURONS = 512;
const int HIDDEN_NEURONS = 2 * SIDE_NEURONS;

const int Q_IN = 4;
const int Q_HIDDEN = 512;

struct NetInput {
    std::vector <short> ind[2];
};

struct Update {
    int16_t ind;
    int8_t coef;
};

class Gradient {
public:
    //float grad;
    float m1, m2; /// momentums

    Gradient(float _m1, float _m2) {
        m1 = _m1;
        m2 = _m2;
        //grad = 0;
    }

    Gradient() {
        m1 = m2 = 0;
        //grad = 0;
    }
};

class Network {
public:

    Network() {
        histSz = 0;

        updateSz[WHITE] = updateSz[BLACK] = 0;

        load();
    }

    // the below functions are taken from here and are used to compute the dot product
    // https://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-sse-vector-sum-or-other-reduction

    float hsum_ps_sse3(__m128 v) {
        __m128 shuf = _mm_movehdup_ps(v);
        __m128 sums = _mm_add_ps(v, shuf);
        shuf = _mm_movehl_ps(shuf, sums);
        sums = _mm_add_ss(sums, shuf);
        return _mm_cvtss_f32(sums);
    }

    float hsum256_ps_avx(__m256 v) {
        __m128 vlow = _mm256_castps256_ps128(v);
        __m128 vhigh = _mm256_extractf128_ps(v, 1);
        vlow = _mm_add_ps(vlow, vhigh);
        return hsum_ps_sse3(vlow);
    }

    int32_t get_sum(__m256i x) {
        __m128i r4 = _mm_add_epi32(_mm256_castsi256_si128(x), _mm256_extractf128_si256(x, 1));
        __m128i r2 = _mm_add_epi32(r4, _mm_srli_si128(r4, 8));
        __m128i r1 = _mm_add_epi32(r2, _mm_srli_si128(r2, 4));

        return _mm_cvtsi128_si32(r1);
    }

    int32_t calc(NetInput& input, bool stm) {
        int16_t sum;

        for (int n = 0; n < SIDE_NEURONS; n++) {
            sum = inputBiases[n];

            for (auto& prevN : input.ind[WHITE]) {
                sum += inputWeights[prevN][n];
            }

            histOutput[WHITE][0][n] = sum;

            sum = inputBiases[n];

            for (auto& prevN : input.ind[BLACK]) {
                sum += inputWeights[prevN][n];
            }

            histOutput[BLACK][0][n] = sum;
        }

        histSz = 1;

        return getOutput(stm);
    }

    int32_t getOutput(NetInput& input, bool stm) { /// feed forward
        int32_t sum;

        for (int n = 0; n < SIDE_NEURONS; n++) {
            sum = inputBiases[n];

            for (auto& prevN : input.ind[WHITE]) {
                sum += inputWeights[prevN][n];
            }

            histOutput[WHITE][0][n] = sum;

            sum = inputBiases[n];

            for (auto& prevN : input.ind[BLACK]) {
                sum += inputWeights[prevN][n];
            }

            histOutput[BLACK][0][n] = sum;

            assert(-32768 <= sum && sum <= 32767);
        }

        sum = outputBias * Q_IN;

        __m256i zero = _mm256_setzero_si256();
        __m256i acc = _mm256_setzero_si256(), acc2 = _mm256_setzero_si256();

        __m256i* v = (__m256i*)outputWeights;
        __m256i* w = (__m256i*)histOutput[stm][0];

        for (int j = 0; j < batches / 2; j++) {
            acc = _mm256_add_epi32(acc, _mm256_madd_epi16(_mm256_max_epi16(w[2 * j], zero), v[2 * j]));
            acc2 = _mm256_add_epi32(acc2, _mm256_madd_epi16(_mm256_max_epi16(w[2 * j + 1], zero), v[2 * j + 1]));
        }

        __m256i* w2 = (__m256i*)histOutput[stm ^ 1][0];

        for (int j = 0; j < batches / 2; j++) {
            acc = _mm256_add_epi32(acc, _mm256_madd_epi16(_mm256_max_epi16(w2[2 * j], zero), v[2 * j + batches]));
            acc2 = _mm256_add_epi32(acc2, _mm256_madd_epi16(_mm256_max_epi16(w2[2 * j + 1], zero), v[2 * j + 1 + batches]));
        }

        acc = _mm256_add_epi32(acc, acc2);

        sum += get_sum(acc);

        return sum / Q_IN / Q_HIDDEN;
    }

    void removeInput(int side, int16_t ind) {
        updates[side][updateSz[side]++] = { ind, -1 };
    }

    void removeInput(int piece, int sq, int kingWhite, int kingBlack) {
        updates[WHITE][updateSz[WHITE]++] = { netInd(piece, sq, kingWhite, WHITE), -1 };
        updates[BLACK][updateSz[BLACK]++] = { netInd(piece, sq, kingBlack, BLACK), -1 };
    }

    void addInput(int side, int16_t ind) {
        updates[side][updateSz[side]++] = { ind, 1 };
    }

    void addInput(int piece, int sq, int kingWhite, int kingBlack) {
        updates[WHITE][updateSz[WHITE]++] = { netInd(piece, sq, kingWhite, WHITE), 1 };
        updates[BLACK][updateSz[BLACK]++] = { netInd(piece, sq, kingBlack, BLACK), 1 };
    }

    void applyUpdates(int c) {
        memcpy(histOutput[c][histSz], histOutput[c][histSz - 1], sizeof(histOutput[c][histSz - 1]));

        __m256i* w = (__m256i*)histOutput[c][histSz];

        for (int i = 0; i < updateSz[c]; i++) {
            __m256i* v = (__m256i*)inputWeights[updates[c][i].ind];

            if (updates[c][i].coef == 1) {
                for (int j = 0; j < batches; j++)
                    w[j] = _mm256_add_epi16(w[j], v[j]);
            }
            else {
                for (int j = 0; j < batches; j++)
                    w[j] = _mm256_sub_epi16(w[j], v[j]);
            }
        }

        updateSz[c] = 0;
    }

    void applyInitUpdates(int c) {
        memcpy(histOutput[c][histSz], inputBiases, sizeof(histOutput[c][histSz]));
        __m256i* w = (__m256i*)histOutput[c][histSz];

        for (int i = 0; i < updateSz[c]; i++) {
            __m256i* v = (__m256i*)inputWeights[updates[c][i].ind];

            if (updates[c][i].coef == 1) {
                for (int j = 0; j < batches; j++)
                    w[j] = _mm256_add_epi16(w[j], v[j]);
            }
            else {
                for (int j = 0; j < batches; j++)
                    w[j] = _mm256_sub_epi16(w[j], v[j]);
            }
        }

        updateSz[c] = 0;
    }

    void revertUpdates() {
        histSz--;
    }

    int32_t getOutput(bool stm) {
        int32_t sum = outputBias * Q_IN;

        __m256i zero = _mm256_setzero_si256();
        __m256i acc = _mm256_setzero_si256();

        __m256i* w = (__m256i*)histOutput[stm][histSz - 1];

        for (int j = 0; j < batches; j++) {
            acc = _mm256_add_epi32(acc, _mm256_madd_epi16(_mm256_max_epi16(w[j], zero), v[j]));
        }

        __m256i* w2 = (__m256i*)histOutput[stm ^ 1][histSz - 1];

        for (int j = 0; j < batches; j++) {
            acc = _mm256_add_epi32(acc, _mm256_madd_epi16(_mm256_max_epi16(w2[j], zero), v[j + batches]));
        }

        sum += get_sum(acc);

        return sum / Q_IN / Q_HIDDEN;
    }

    void load() {
        int* intData;
        float* floatData;
        Gradient* gradData;
        Gradient kekw;
        std::vector <float> w;

        int x;
        intData = (int*)gNetData;

        x = *(intData++);
        assert(x == 3);

        floatData = (float*)intData;

        int sz;

        sz = SIDE_NEURONS;

        for (int j = 0; j < sz; j++) {
            float val = *(floatData++);
            inputBiases[j] = round(val * Q_IN);
        }

        gradData = (Gradient*)floatData;

        for (int j = 0; j < sz; j++)
            kekw = *(gradData++);

        floatData = (float*)gradData;

        int mn = (int)1e9, mx = (int)-1e9;
        for (int i = 0; i < SIDE_NEURONS * INPUT_NEURONS; i++) {
            float val = *(floatData++);
            inputWeights[i / SIDE_NEURONS][i % SIDE_NEURONS] = round(val * Q_IN);
        }

        gradData = (Gradient*)floatData;

        for (int i = 0; i < SIDE_NEURONS * INPUT_NEURONS; i++) {
            kekw = *(gradData++);
        }

        sz = 1;
        floatData = (float*)gradData;

        outputBias = round(*(floatData++) * Q_HIDDEN);

        gradData = (Gradient*)floatData;

        kekw = *(gradData++);

        floatData = (float*)gradData;

        for (int j = 0; j < HIDDEN_NEURONS; j++) {
            float val = *(floatData++);
            outputWeights[j] = round(val * Q_HIDDEN);
        }
    }

    int lg = sizeof(__m256i) / sizeof(int16_t);
    int batches = SIDE_NEURONS / lg;

    int histSz, updateSz[2];

    int16_t inputBiases[SIDE_NEURONS] __attribute__((aligned(32)));
    int32_t outputBias;
    int16_t histOutput[2][2005][SIDE_NEURONS] __attribute__((aligned(32)));
    int16_t inputWeights[INPUT_NEURONS][SIDE_NEURONS] __attribute__((aligned(32)));
    int16_t outputWeights[HIDDEN_NEURONS] __attribute__((aligned(32)));
    int16_t deeznuts[HIDDEN_NEURONS] __attribute__((aligned(32)));

    __m256i* v = (__m256i*)outputWeights;

    Update updates[2][105];
};