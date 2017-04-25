#ifndef XT_HASHDETECTOR_H_
#define XT_HASHDETECTOR_H_

#include "xt_blockdetector.h"

class HashDetector: public BlockDetector{
public:
  HashDetector(uint32_t in_begin, uint32_t in_len,
               uint32_t out_begin, uint32_t out_len)
    : BlockDetector(in_begin, in_len, out_begin, out_len) {}

  // if:
  //  1) total range of in buffer have avalanche to a subrange of output buffer
  //  2) the subrange > 20 bytes
  // we consider it is a hash function
  bool detect_hash(RangeArray      &in_blocks,
                   VSPtrRangeArray &in_blocks_prpgt,
                   const std::vector<TaintPropagate *>& in_prpgt_ra_res);
};

#endif /* XT_HASHDETECTOR_H_ */
