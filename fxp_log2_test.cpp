
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "fxp_log2_top.h"

#define NUM_TEST_ITERS 100 // 0000
#define MIN_ITER_IDX 0
#define MAX_ITER_IDX (MIN_ITER_IDX + NUM_TEST_ITERS)

#define MY_DRAND() (rand() * rand() / (double)(RAND_MAX * RAND_MAX + 1))

// #define ABS_ERR_THRESH (0.0 / (double)(1ll << (OUT_BW - OUT_IW)))
#define ABS_ERR_THRESH 0.1

// Test program for validating C-model functionality and RTL co-simulation
int main(int argc, char* argv[]) {
    in_data_t test_val;
    unsigned err_cnt = 0;


    for (uint32_t i = MIN_ITER_IDX; i < MAX_ITER_IDX; i++) {
        test_val.range(IN_BW - 1, 0) = rand();

        // Run theVivado HLS top-level function
        out_data_t hw_outval = fxp_log2_top(test_val);

        // Check value against floating point reference value rounded to input
        // format
        ap_ufixed<OUT_BW, OUT_IW, AP_RND> sw_outval =
            std::log2(test_val.to_double());

        if (NUM_TEST_ITERS <= 100) {
            cout << "log2(" << test_val << ") = " << sw_outval << ";\t";
            cout << "fxp_log2(" << test_val << ") = " << hw_outval << endl;
        } else {
            if (i == 0)
                cout << "Running test.";
            else if (!(i % (NUM_TEST_ITERS / 100))) {
                cout << ".";
                fflush(stdout);
            }
        }
        if (fabs(hw_outval.to_double() - sw_outval.to_double()) >
            ABS_ERR_THRESH) {
            cout << "MISMATCH (" << i << "): \t";
            cout << "fxp_log2(" << test_val << ") = " << hw_outval;
            cout << "\tDelta = "
                 << (hw_outval.to_double() - sw_outval.to_double());
            cout << "\thw_outval/sw_outval = "
                 << (hw_outval.to_double() / sw_outval.to_double());
            cout << endl << endl;
            err_cnt++;
        }
    }

    cout << endl;
    if (err_cnt) {
        cout << "!!! ERROR: " << err_cnt << " mismatches detected !!!";
        cout << endl << endl;
    } else {
        cout << "*** Test passes ***" << endl << endl;
    }
    if (err_cnt)
        return 1;
    else
        return 0;
}
