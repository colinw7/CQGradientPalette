#ifndef CCOLOR_RANGE_H
#define CCOLOR_RANGE_H

#include <vector>
#include <CRGB.h>

class CColorRange {
 private:
  std::vector<CRGB> colors_;

 public:
  CColorRange();

 ~CColorRange() { }

  const CRGB &getColor(uint i) const {
    return colors_[i];
  }

 private:
  void init();
};

#endif
