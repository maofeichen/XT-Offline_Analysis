#ifndef XT_RANGEARRAY_H_
#define XT_RANGEARRAY_H_

#include "xt_range.h"
#include <cstdint>
#include <map>

class RangeArray{
public:
  RangeArray() { init(); }
  RangeArray(uint32_t begin, uint32_t len);
  RangeArray(const Range& r);
  ~RangeArray();

  Range* operator[](uint32_t i);
  const Range* operator[](uint32_t i) const;
  Range* at(uint32_t i);
  const Range* at(uint32_t i) const;
  RangeArray& operator=(const RangeArray& rhs);

  void add_range(const Range& r);
  void add_range(uint32_t begin, uint32_t len);
  void add_range(uint32_t begin, uint32_t len,
                 const std::multimap<uint32_t,uint32_t>& byte_val_map);

  RangeArray& get_common_range(RangeArray& rhs);
  void        get_common_range(RangeArray& ra_right, RangeArray& common);

  uint32_t get_size() const { return array_use_; }

  void print_range_array() const;
private:
  Range **ref_array_;
  uint32_t array_sz_;
  uint32_t array_use_;

  void copy(const RangeArray& rhs);
  void copy_with_val(const RangeArray& rhs);

  void init();
  void reset();

  void insert_range(int pos, const Range& r);
  void insert_range(int pos,
                    uint32_t begin,
                    uint32_t len);
  void insert_range(int pos,
                    uint32_t begin,
                    uint32_t len,
                    const std::multimap<uint32_t,uint32_t>& byte_val_map);

  void get_byte_val_map(const Range& r,
                        uint32_t r_begin,
                        uint32_t r_len,
                        std::multimap<uint32_t,uint32_t>& map_res);
};

#endif /* XT_RANGEARRAY_H_ */
