#ifndef _KOVAN_COLINEAR_SEGMENTER_HPP_
#define _KOVAN_COLINEAR_SEGMENTER_HPP_

#include "segment.hpp"

#include <list>

class ColinearSegmenter : public Segmenter<int>
{
public:
  ColinearSegmenter(const type threshold);
  
  virtual std::vector<Segment> findSegments(const type *const data, const unsigned size) const;
    
private:
  const type _threshold;
};

#endif