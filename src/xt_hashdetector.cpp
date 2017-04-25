#include "xt_hashdetector.h"
#include <iostream>

using namespace std;

//HashDetector::HashDetector(uint32_t in_begin, uint32_t in_len,
//                           uint32_t out_begin, uint32_t out_len)
//{
//  in_begin_   = in_begin;
//  in_len_     = in_len;
//  out_begin_  = out_begin;
//  out_len_    = out_len;
//}

bool
HashDetector::detect_hash(RangeArray      &in_blocks,
                          VSPtrRangeArray &in_blocks_prpgt,
                          const vector<TaintPropagate *>& in_prpgt_ra_res)
{
  RangeArray common(out_begin_, out_len_);
  RangeArray old_common;

  uint32_t i = 0;
  if(!init_block(common, i, i+1, in_prpgt_ra_res) ) {
    cout << "hash detection: there is common between first two given bytes." << endl;
    return false;
  }

  common.print_range_array();
  for(; i < in_len_; i++) {

  }
}
