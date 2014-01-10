#ifndef _KOVAN_SEGMENT_HPP_
#define _KOVAN_SEGMENT_HPP_

#include <vector>

struct Segment
{
  unsigned start;
  unsigned end;
  unsigned startValue;
  unsigned endValue;
};

template<typename T>
class Segmenter
{
public:
  typedef T type;
  virtual std::vector<Segment> findSegments(const T *const data, const unsigned size) const = 0;
};

std::vector<Segment> coalesceSegments(const std::vector<Segment> &segments);

#endif