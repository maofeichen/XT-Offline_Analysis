#include "xt_ctrdetector.h"
#include <iostream>
using namespace std;

bool
CTRDetector::detect_ctr(const std::vector<TaintPropagate *>& in_prpgt_ra_res)
{
  uint32_t accumu_sz  = 0;
  uint32_t i = 0;

  for(; i < in_len_; i++) {
    if(has_byte_to_byte(i, in_prpgt_ra_res, out_begin_+accumu_sz) ) {

    } else {
      return false;
    }

    accumu_sz++;
  }

  uint32_t in_res_begin = compt_in_res_begin(0);
  res_in.set_begin(in_res_begin);
  res_in.set_end(in_res_begin+i);

  res_out.set_begin(out_begin_);
  res_out.set_end(out_begin_+out_len_);

//  res_in.print_range();
//  res_out.print_range();

  return true;
}

bool
CTRDetector::has_byte_to_byte(uint32_t idx_byte,
                              const vector<TaintPropagate *>& in_prpgt_ra_rs,
                              uint32_t prpgt_addr)
{
  if(idx_byte > in_prpgt_ra_rs.size() - 1) {
    cout << "ctr: has byte to byte: given byte idx is invalid." << endl;
    return false;
  }

  TaintPropagate* byte = in_prpgt_ra_rs[idx_byte];
//  byte->get_taint_propagate()->print_range_array();
  if(byte->get_taint_propagate()->get_size() == 0) {
    cout << "ctr: has byte to byte: given byte has no propagation." << endl;
    return false;
  }

  RangeArray common(out_begin_, out_len_);
  common.get_common_range(*byte->get_taint_propagate() );
//  common.print_range_array();
  if(common.get_size() == 0) {
    cout << "ctr: has byte to byte: given byte has no common with output." << endl;
    return false;
  }

  for(uint32_t i = 0; i < common.get_size(); i++) {
    if(common[i]->has_range(prpgt_addr, 1) ) {
      return true;
    }
  }

  cout << "ctr: has byte to byte: given byte can't propagate to target addr." << endl;
  return false;
}
