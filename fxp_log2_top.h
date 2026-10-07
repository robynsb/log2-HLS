#ifndef FXP_LOG2_TOP_H_
#define FXP_LOG2_TOP_H_

#include "fxp_log2.h"
#include <ap_int.h>

#define IN_BW 24
#define OUT_BW 24
#define OUT_IW 9

// typedefs for top-level input and output fixed-point formats
typedef ap_uint<IN_BW> in_data_t;
typedef ap_ufixed<OUT_BW, OUT_IW> out_data_t;

// Top level wrapper function - calls the core template function w/ above types
out_data_t fxp_log2_top(in_data_t& in_val);

#endif // FXP_LOG2_TOP_H_ not defined
