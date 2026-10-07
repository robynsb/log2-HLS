#include "fxp_log2_top.h"

out_data_t fxp_log2_top(in_data_t& in_val) {
    out_data_t result;
    fxp_log2(result, in_val);
    return result;
}
