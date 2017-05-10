#ifndef XT_CTRDETECTOR_H_
#define XT_CTRDETECTOR_H_

#include "xt_streamcipherdetector.h"

class CTRDetector : public StreamDetecor{
public:
  CTRDetector(uint32_t in_begin, uint32_t in_len,
              uint32_t out_begin, uint32_t out_len)
    : StreamDetecor(in_begin, in_len, out_begin, out_len) {}

  // if each byte of input can propagate to each byte of output
  bool detect_ctr(const std::vector<TaintPropagate *>& in_prpgt_ra_res);
private:
  bool has_byte_to_byte(uint32_t idx_byte,
                        const std::vector<TaintPropagate *>& in_prpgt_ra_rs,
                        uint32_t prpgt_addr);
};

#endif /* XT_CTRDETECTOR_H_ */
