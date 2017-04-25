#ifndef XT_TYPE_H_
#define XT_TYPE_H_

#include <memory>
#include <vector>
#include "xt_rangearray.h"

typedef std::shared_ptr<RangeArray> RangeArraySPtr;
typedef std::vector<RangeArraySPtr> VSPtrRangeArray;

#endif /* XT_TYPE_H_ */
