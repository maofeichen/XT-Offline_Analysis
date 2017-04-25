#include "xt_rangearray.h"
#include <stdexcept>
#include <iostream>

using namespace std;

RangeArray::RangeArray(uint32_t begin, uint32_t len)
{
  init();
  add_range(begin, len);
}

RangeArray::RangeArray(const Range& r)
{
  init();
  add_range(r);
}

RangeArray::~RangeArray()
{
  reset();
  delete[] ref_array_;
}

Range*
RangeArray::operator[](uint32_t i)
{
  return at(i);
}

const Range*
RangeArray::operator[](uint32_t i) const
{
  return at(i);
}

Range*
RangeArray::at(uint32_t i)
{
  if(i > array_use_-1) { return NULL; }
  else { return ref_array_[i]; }
}

const Range*
RangeArray::at(uint32_t i) const
{
  if(i > array_use_-1) { return NULL; }
  else { return ref_array_[i]; }
}


RangeArray&
RangeArray::operator=(const RangeArray& rhs)
{
  if(this != &rhs) {
    copy_with_val(rhs);
  }
  return *this;
}

void
RangeArray::add_range(const Range& r)
{
  // binary searches
  int low    = 0;
  int mid    = low;
  int hi     = array_use_;

  while(low < hi) {
    mid = low + (hi-low) / 2;
    if(ref_array_[mid]->get_begin() < r.get_begin() ) {
      low = mid + 1;
    }else {
      hi  = mid;
    }
  }

  insert_range(low, r);
}

void
RangeArray::add_range(uint32_t begin, uint32_t len)
{
  // binary searches
  int low    = 0;
  int mid    = low;
  int hi     = array_use_;

  while(low < hi) {
    mid = low + (hi-low) / 2;
    if(ref_array_[mid]->get_begin() < begin) {
      low = mid + 1;
    }else {
      hi  = mid;
    }
  }

  insert_range(low, begin, len);
}

void
RangeArray::add_range(uint32_t begin, uint32_t len,
                      const std::multimap<uint32_t,uint32_t>& byte_val_map)
{
  // binary searches
  int low    = 0;
  int mid    = low;
  int hi     = array_use_;

  while(low < hi) {
    mid = low + (hi-low) / 2;
    if(ref_array_[mid]->get_begin() < begin) {
      low = mid + 1;
    }else {
      hi  = mid;
    }
  }

  insert_range(low, begin, len, byte_val_map);
}

RangeArray&
RangeArray::get_common_range(RangeArray& rhs)
{
  RangeArray common;
  get_common_range(rhs, common);

  *this = common;
  return *this;
}

void
RangeArray::get_common_range(RangeArray& ra_right, RangeArray& common)
{
  int idx_left = 0;
  int idx_right = 0;

  Range*     r_left;
  Range*     r_right;
  RangeArray &ra_left = *this;

  common.reset();

  // finds the common ranges of left and right range
  while(true) {
    r_left  = ra_left[idx_left];
    r_right = ra_right[idx_right];

    if(r_left == NULL || r_right == NULL) { break; }

    // chooses the larger begin addr
    // chooses the smaller end addr
    // which makes it common range between left right
    uint32_t c_begin  = max(r_left->get_begin(), r_right->get_begin() );
    uint32_t c_end    = min(r_left->get_end(), r_right->get_end() );

    if(c_begin < c_end) {
      // it is the common range
      multimap<uint32_t,uint32_t> byte_val_map;
      get_byte_val_map(*r_right, c_begin, c_end, byte_val_map);
      common.add_range(c_begin, c_end-c_begin, byte_val_map);
//      common.print_range_array();
    }

    // if left range is smaller than right range, increase it
    // notices that all ranges in range array are in increasing order
    if(r_left->get_end() < r_right->get_end() ) {
      idx_left++;
    } else if(r_left->get_end() > r_right->get_end() ) {
      idx_right++;
    } else {
      idx_left++;
      idx_right++;
    }
  }

//  for(uint32_t i = 0; i < common.get_size(); i++) {
//    common[i]->print_range();
//    common[i]->print_byte_val_map();
//  }
}

void
RangeArray::print_range_array() const
{
  for(uint32_t i = 0; i < array_use_; i++) {
    ref_array_[i]->print_range();
  }
}


void
RangeArray::copy_with_val(const RangeArray& rhs)
{
  reset();

  if(array_sz_ < rhs.get_size() ) {
    array_sz_   = rhs.get_size();
    delete[]    ref_array_;
    ref_array_  = new Range* [array_sz_];
  }

  for(uint32_t i = 0; i < rhs.get_size(); i++) {
    const Range* old  = rhs[i];
    Range* copy       = new Range(old->get_begin(), old->get_len(),
                                  old->get_byte_val_map() );

    ref_array_[i] = copy;
    array_use_++;
  }
}


void
RangeArray::init()
{
  array_sz_   = 8;
  array_use_  = 0;
  ref_array_  = new Range* [array_sz_];
}

void RangeArray::reset()
{
  for(uint32_t i = 0; i < array_use_; i++) {
    delete ref_array_[i];
  }
  array_use_ = 0;
}


void
RangeArray::insert_range(int pos, const Range& r)
{
  if(pos > array_use_) {
    throw runtime_error("insert range: pos is larger than used.");
  }

  Range* range = new Range(r);

  if(array_use_+1 > array_sz_) {
    // grows size
    array_sz_ += array_sz_;
    Range** new_array = new Range* [array_sz_];

    for(int i = 0; i < pos; i++) {
      new_array[i] = ref_array_[i];
    }

    new_array[pos] = range;

    for(int i = pos+1; i < array_use_ + 1; i++) {
      new_array[i] = ref_array_[i-1];
    }

    delete[] ref_array_;
    ref_array_ = new_array;
  }else {
    for(int i = array_use_-1; i >= pos; i--) {
      ref_array_[i+1] = ref_array_[i];
    }
    ref_array_[pos] = range;
  }

  array_use_++;
}

void
RangeArray::insert_range(int pos,
                         uint32_t begin,
                         uint32_t len)
{
  if(pos > array_use_) {
    throw runtime_error("insert range: pos is larger than used.");
  }

  Range* range = new Range(begin, len);

  if(array_use_+1 > array_sz_) {
    // grows size
    array_sz_ += array_sz_;
    Range** new_array = new Range* [array_sz_];

    for(int i = 0; i < pos; i++) {
      new_array[i] = ref_array_[i];
    }

    new_array[pos] = range;

    for(int i = pos+1; i < array_use_ + 1; i++) {
      new_array[i] = ref_array_[i-1];
    }

    delete[] ref_array_;
    ref_array_ = new_array;
  }else {
    for(int i = array_use_-1; i >= pos; i--) {
      ref_array_[i+1] = ref_array_[i];
    }
    ref_array_[pos] = range;
  }

  array_use_++;
}

void
RangeArray::insert_range(int pos,
                         uint32_t begin,
                         uint32_t len,
                         const multimap<uint32_t,uint32_t> &byte_val_map)
{
  if(pos > array_use_) {
    throw runtime_error("insert range: pos is larger than used.");
  }

  Range* range = new Range(begin, len, byte_val_map);

  if(array_use_+1 > array_sz_) {
    // grows size
    array_sz_ += array_sz_;
    Range** new_array = new Range* [array_sz_];

    for(int i = 0; i < pos; i++) {
      new_array[i] = ref_array_[i];
    }

    new_array[pos] = range;

    for(int i = pos+1; i < array_use_ + 1; i++) {
      new_array[i] = ref_array_[i-1];
    }

    delete[] ref_array_;
    ref_array_ = new_array;
  }else {
    for(int i = array_use_-1; i >= pos; i--) {
      ref_array_[i+1] = ref_array_[i];
    }
    ref_array_[pos] = range;
  }

  array_use_++;
}

void
RangeArray::get_byte_val_map(const Range& r,
                             uint32_t r_begin,
                             uint32_t r_len,
                             std::multimap<uint32_t,uint32_t>& map_res)
{
  uint32_t r_end = r_begin + r_len;
  if(r.get_begin() > r_begin
     || r.get_end() > r_end) {
    throw runtime_error("err: get byte val map - given invalid ranges");
  } else {
    for(uint32_t i = r_begin; i < r_len; i++) {
      pair<multimap<uint32_t,uint32_t>::const_iterator,
           multimap<uint32_t,uint32_t>::const_iterator> ret;
      ret = r.get_byte_val_map().equal_range(i);

      for(multimap<uint32_t,uint32_t>::const_iterator it = ret.first;
          it != ret.second; ++it) {
        map_res.insert(*it);
      }
    }

    for(auto it = map_res.begin(); it != map_res.end(); ++it) {
      cout << "addr: " << hex << it->first
           << " val "  << hex << it->second << endl;
    }
  } // end else
}
