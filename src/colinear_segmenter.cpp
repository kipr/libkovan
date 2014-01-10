#include <kovan/colinear_segmenter.hpp>
#include <iostream>

static ColinearSegmenter::type cabs(const ColinearSegmenter::type v)
{
  if(v < 0) return -v;
  return v;
}

ColinearSegmenter::ColinearSegmenter(const ColinearSegmenter::type threshold)
  : _threshold(threshold)
{
}

std::vector<Segment> ColinearSegmenter::findSegments(const ColinearSegmenter::type *const data,
  const unsigned size) const
{
  std::vector<Segment> ret;
  int start = 0;
  for(int i = 2; i < size; ++i) {
    const ColinearSegmenter::type s = (data[i - 1] - data[start]) / (i - 1 - start);
    if(cabs(data[i] - data[i - 1] - s) <= _threshold) continue;
    if(i - start > 2) {
      Segment seg;
      seg.start = start;
      seg.end = i - 1;
      seg.startValue = data[seg.start];
      seg.endValue = data[seg.end];
      ret.push_back(seg);
    }
    start = i - 1;
  }
  if(size - start > 2) {
    Segment seg;
    seg.start = start;
    seg.end = size - 1;
    seg.startValue = data[seg.start];
    seg.endValue = data[seg.end];
    ret.push_back(seg);
  }
  
  return ret;
}

