#include "xt_streamcipherdetector.h"

using namespace std;

StreamDetecor::StreamDetecor(uint32_t in_begin, uint32_t in_len,
                             uint32_t out_begin, uint32_t out_len)
{
  in_begin_   = in_begin;
  in_len_     = in_len;
  out_begin_  = out_begin;
  out_len_    = out_len;
}


void
StreamDetecor::detect_stream(const vector<TaintPropagate *>& in_prpgt_ra_res){}

uint32_t
StreamDetecor::compt_in_res_begin(uint32_t idx_begin)
{
  if(idx_begin > in_len_) {
    throw runtime_error("compute in res begin: given index is invalid.");
  }
  return in_begin_ + idx_begin;
}
