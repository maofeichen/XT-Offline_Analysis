#include "xt_constant.h"
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

  old_common  = common;

  for(i += 2; i < in_len_; i++) {
//    old_common.print_range_array();
//    common.print_range_array();

    if(extend_block(common, i, in_prpgt_ra_res) ) {
      if(has_same_prgpt(old_common, common) ) {
        old_common  = common;
      } else {
       cout << "detect hash: old common and common are different." << endl;
       return false;
      }
    } else {
      cout << "detect hash: given byte has no common propagation." << endl;
      return false;
    }
  }

  if(i == in_len_
     && is_hash_sz(common) ) {

    uint32_t in_res_begin = compt_in_res_begin(0);
    res_in.set_begin(in_res_begin);
    res_in.set_end(in_res_begin+i);

    res_out.set_begin(common[0]->get_begin() );
    res_out.set_end(common[0]->get_end() );

//    res_in.print_range();
//    res_out.print_range();

    return true;
  } else {
    return false;
  }
}

bool
HashDetector::has_same_prgpt(const RangeArray& old_common,
                             const RangeArray& common)
{
  if(old_common.get_size() == 0
     || common.get_size() == 0) {
    cout << "has same propagation: given range arraies are not same" << endl;
    return false;
  }

  // if first ranges are same, consider same
  if(old_common[0]->get_begin() == common[0]->get_begin()
     && old_common[0]->get_end() == common[0]->get_end() ) {
    return true;
  }else {
    cout << "has same propagation: given range arraies are not same" << endl;
    return false;
  }
}

bool
HashDetector::is_hash_sz(const RangeArray& common)
{
  switch(common[0]->get_len() ) {
    case xt_const::SHA1:
    case xt_const::SHA224:
    case xt_const::SHA256:
    case xt_const::SHA384:
    case xt_const::SHA512:
      return true;
  }

  return false;
}
