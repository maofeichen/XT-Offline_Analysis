#include "xt_blockdetector.h"
#include <iostream>
#include <stdexcept>

using namespace std;

BlockDetector::BlockDetector(uint32_t in_begin, uint32_t in_len,
                             uint32_t out_begin, uint32_t out_len)
{
  in_begin_   = in_begin;
  in_len_     = in_len;
  out_begin_  = out_begin;
  out_len_    = out_len;
}

void
BlockDetector::detect_block(RangeArray &in_blocks,
                            VSPtrRangeArray &in_blocks_prpgt,
                            const vector<TaintPropagate *>& in_prpgt_ra_res)
{
  detect_block_not_handle_last(in_blocks, in_blocks_prpgt, in_prpgt_ra_res);
}

void
BlockDetector::detect_block_not_handle_last(RangeArray &in_blocks,
                                            VSPtrRangeArray &in_blocks_prpgt,
                                            const vector<TaintPropagate *>& in_prpgt_ra_res)
{
  uint32_t bk_begin_byte  = 0;
  uint32_t bk_end_byte    = in_len_;

  uint32_t buf_size       = in_len_;
  uint32_t accumu_bk_sz   = 0;
  uint32_t bk_sz          = 0;

  // loops until reaching begin of potential last blcok
  while(buf_size - accumu_bk_sz > bk_sz) {
    RangeArray common(out_begin_, out_len_);
    RangeArray old_common;

    uint32_t i = bk_begin_byte;
    for(; i < bk_end_byte; i++) {
      common.print_range_array();
      old_common.print_range_array();

      if (accumu_bk_sz == 0) {
        // Handles block begins, for a potential block, uses the first
        // two byte propagations to setup the common ranges
        bool is_init  = init_block(common, i, i+1, in_prpgt_ra_res);

        if(!is_init
           || common.get_size() == 0) {
          cout << "potential block first two bytes have no common." << endl;
          bk_begin_byte = i;
          buf_size--;
          break;
        } else {
          // inits block successfully
          old_common = common;
          accumu_bk_sz++;
        }
      } else {
        bool is_ext = extend_block(common, i, in_prpgt_ra_res);

        if(!is_ext) {
          save_block(in_blocks, in_blocks_prpgt, bk_begin_byte, accumu_bk_sz, old_common);

          bk_begin_byte = i;
          buf_size      -= accumu_bk_sz;
          accumu_bk_sz  = 0;
          break;

        } else {
          // extends the current byte successfully
          if(is_block_end(common, old_common, accumu_bk_sz) ) {
            save_block(in_blocks, in_blocks_prpgt, bk_begin_byte, accumu_bk_sz, old_common);

            bk_begin_byte   = i;
            buf_size        -= accumu_bk_sz;
            accumu_bk_sz    = 0;
            break;

          } else {
            old_common = common;
            accumu_bk_sz++;
          }
        } // end if is_ext
      }
    } // end for loop

    // updates block size if any
    if(in_blocks.get_size() > 0) {
      uint32_t num_bk = in_blocks.get_size();
      bk_sz           = in_blocks.at(num_bk-1)->get_len();
    }
  } // end while

  in_blocks.add_range(bk_begin_byte, buf_size);
}

bool
BlockDetector::init_block(RangeArray &common,
                          uint32_t idx_byte_a,
                          uint32_t idx_byte_b,
                          const vector<TaintPropagate *>& in_prpgt_ra_res)
{
  uint32_t buf_sz = in_prpgt_ra_res.size();
  if(idx_byte_a > buf_sz - 1
     || idx_byte_b > buf_sz - 1) {
    throw runtime_error("init block: given invalid index");
  }

  TaintPropagate *a = in_prpgt_ra_res[idx_byte_a];
  TaintPropagate *b = in_prpgt_ra_res[idx_byte_b];

  if(a->get_taint_propagate()->get_size() == 0
     || b->get_taint_propagate()->get_size() == 0) {
    cout << "init block: given byte proagations are empty." << endl;
    return false;
  }

  common.get_common_range(*a->get_taint_propagate() );
//  common.print_range_array();
  if(common.get_size() == 0) {
    cout << "init block: given first byte propagation is empty." << endl;
    return false;
  }else {
    common.get_common_range(*b->get_taint_propagate() );
//    common.print_range_array();
  }

  if(common.get_size() == 0) {
    cout << "init block: given first byte propagation is empty." << endl;
    return false;
  } else {
    return true;
  }
}

bool
BlockDetector::extend_block(RangeArray &common,
                            uint32_t idx_byte,
                            const vector<TaintPropagate *>& in_prpgt_ra_res)
{
  uint32_t buf_sz = in_prpgt_ra_res.size();
  if(idx_byte > buf_sz - 1) {
    throw runtime_error("init block: given invalid index");
  }

  TaintPropagate* byte = in_prpgt_ra_res[idx_byte];

  if(byte->get_taint_propagate()->get_size() == 0) {
    cout << "init block: given byte propagation is empty." << endl;
    return false;
  }else {
    common.get_common_range(*byte->get_taint_propagate() );
    if(common.get_size() == 0) {
      return false;
    } else {
      return true;
    }
  }
}

bool
BlockDetector::save_block(RangeArray      &in_blocks,
                          VSPtrRangeArray &in_blocks_prpgt,
                          uint32_t        bk_begin_idx,
                          uint32_t        accumu_bk_sz,
                          RangeArray      &common)
{

}

bool
BlockDetector::is_block_end(RangeArray &common,
                            RangeArray &old_common,
                            uint32_t   accumu_b_sz)
{

}

uint32_t
BlockDetector::compt_in_res_begin(uint32_t idx_begin)
{
  if(idx_begin > in_len_) {
    throw runtime_error("compute in res begin: given index is invalid.");
  }
  return in_begin_ + idx_begin;
}
