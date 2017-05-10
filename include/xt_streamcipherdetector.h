#ifndef XT_STREAMCIPHERDETECTOR_H_
#define XT_STREAMCIPHERDETECTOR_H_

#include "xt_rangearray.h"
#include "xt_taintpropagate.h"
#include <vector>

class StreamDetecor{
public:
  Range res_in;
  Range res_out;

  StreamDetecor(uint32_t in_begin, uint32_t in_len,
                uint32_t out_begin, uint32_t out_len);

  void detect_stream(const std::vector<TaintPropagate *>& in_prpgt_ra_res);
protected:
  uint32_t in_begin_;
  uint32_t in_len_;
  uint32_t out_begin_;
  uint32_t out_len_;

  uint32_t compt_in_res_begin(uint32_t idx_begin);
};

#endif /* XT_STREAMCIPHERDETECTOR_H_ */
