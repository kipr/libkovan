#include <kovan/segment.hpp>
#include <iostream>

std::vector<Segment> coalesceSegments(const std::vector<Segment> &segments)
{
  using namespace std;
  
  vector<Segment> ret = segments;
  if(ret.size() <= 1) return ret;
  
  std::vector<Segment>::iterator last = ret.begin();
  std::vector<Segment>::iterator it = last + 1;
  for(; it != ret.end(); ++it) {
    const unsigned depth = std::abs((long)(*it).startValue - (*last).endValue);
    const int lastDiff = (int)(*last).startValue - (int)(*last).endValue;
    const int itDiff = (int)(*it).startValue - (int)(*it).endValue;
    if((*it).start - (*last).end <= 5 && depth < 10 && lastDiff > itDiff) {
      Segment seg;
      seg.start = (*last).start;
      seg.end = (*it).end;
      seg.startValue = (*last).startValue;
      seg.endValue = (*it).endValue;
      
      last = ret.insert(ret.erase(ret.erase(last)), seg);
      it = last + 1;
      --it;
    }
    
    last = it;
  }

  return ret;
}