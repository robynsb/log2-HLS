#ifndef __FXP_LOG2_H__
#define __FXP_LOG2_H__

#include <ap_fixed.h>
#include <cassert>
#include <cmath>

using namespace std;

constexpr int ceil_log2(int x) {
    int r = 0;
    int p = 1;

    while (p < x) {
        p <<= 1;
        ++r;
    }

    return r;
}

#define LUT_BW 5
#define LUT_SIZE (1 << LUT_BW)
#define LUT_ROWS (LUT_SIZE + 1)

#define INTERP_BW  2

template <int FW2>
void init_log2_table(ap_uint<FW2 + 1> lut[LUT_ROWS])
{
    for (int i = 0; i < LUT_ROWS; i++) {
        double x = 1.0 + (double)i / LUT_SIZE;
        double log_val = std::log2(x);

        lut[i] = (ap_uint<FW2 + 1>)
            (log_val * (1ULL << FW2) + 0.5);
    }
}

/*
 * Uses log2​(x)=log2​(2^e * m) = e + log2​(m)
 * NO INTERPOLATION
*/
// template <int W2, int IW2, int W1>
// void fxp_log2(ap_ufixed<W2, IW2>& result, ap_uint<W1>& in_val) {
//     static_assert(IW2 >= ceil_log2(W1),
//                   "IW2 must be >= ceil(log2(W1))");
    
//     const int FRAC_BITS = W2 - IW2;

//     ap_uint<FRAC_BITS + 1> log2_table[LUT_ROWS];
//     init_log2_table<FRAC_BITS>(log2_table);

//     if(in_val == 0) {
//         result = 0;
//         return;
//     }

//     ap_uint<IW2> e = 0;

//     e = W1 - 1 - in_val.countLeadingZeros();

//     result = e;

//     ap_uint<W1> remainder = in_val - (ap_uint<W1>(1) << e);

//     ap_uint<LUT_BW> LUT_index;
//     if(LUT_BW > e) {
//         LUT_index = remainder << (LUT_BW - e);
//     } else {
//         LUT_index = remainder >> (e - LUT_BW);
//     }

//     // result.range(W2 - 1, W2 - IW2) = e;
//     result.range(FRAC_BITS - 1, 0) = log2_table[LUT_index];
// }

/*
 * Uses log2​(x)=log2​(2^e * m) = e + log2​(m)
 * w/ INTERPOLATION
*/
template <int W2, int IW2, int W1>
void fxp_log2(ap_ufixed<W2, IW2>& result, ap_uint<W1>& in_val) {
    static_assert(IW2 >= ceil_log2(W1),
                  "IW2 must be >= ceil(log2(W1))");
    
    const int FRAC_BITS = W2 - IW2;

    ap_uint<FRAC_BITS + 1> log2_table[LUT_ROWS];
    init_log2_table<FRAC_BITS>(log2_table);

    if(in_val == 0) {
        result = 0;
        return;
    }

    ap_uint<IW2> e = 0;

    e = W1 - 1 - in_val.countLeadingZeros();

    result = e;

    ap_uint<W1> remainder = in_val - (ap_uint<W1>(1) << e);

    const int NORM_BW = LUT_BW + INTERP_BW;

    ap_uint<NORM_BW> normalized = 0;

    if (e >= NORM_BW) {
        normalized =
            remainder.range(e - 1, e - NORM_BW);
    }
    else {
        normalized.range(e - 1, 0) =
            remainder.range(e - 1, 0);
    }

    ap_uint<LUT_BW> LUT_index =
        normalized.range(NORM_BW - 1, INTERP_BW);

    ap_uint<INTERP_BW> INTERP_index =
        normalized.range(INTERP_BW - 1, 0);

    ap_uint<FRAC_BITS + 1> y0 = log2_table[LUT_index];
    ap_uint<FRAC_BITS + 1> y1 = log2_table[LUT_index + 1];

    ap_uint<FRAC_BITS + 2> delta = y1 - y0;

    ap_uint<FRAC_BITS + INTERP_BW + 1> product =
        delta * INTERP_index;

    ap_uint<FRAC_BITS + 1> interpolation =
        product >> INTERP_BW;

    ap_uint<FRAC_BITS + 1> fractional_log =
        y0 + interpolation;

    result.range(FRAC_BITS - 1, 0) = fractional_log;
}
#endif //__FXP_LOG2_H__ not defined
