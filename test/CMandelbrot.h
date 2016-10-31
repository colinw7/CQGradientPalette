#ifndef CMANDELBROT_H
#define CMANDELBROT_H

#include <CPointFractal.h>

class CMandelbrot : public CPointFractalCalc {
 public:
  CMandelbrot();
  CMandelbrot(const CMandelbrot &m);

  CMandelbrot *dup() const;

  double getXMin() const { return -2.0; }
  double getYMin() const { return -1.2; }
  double getXMax() const { return  1.2; }
  double getYMax() const { return  1.2; }

  void setDistance(bool distance) { distance_ = distance; }

  void initCalc(int pixel_xmin, int pixel_ymin, int pixel_xmax, int pixel_ymax,
                double xmin=-2.0, double ymin=-1.2, double xmax=1.2, double ymax=1.2,
                int max_iterations=1000);

  int calc(double x, double y, int max_iterations) const;

  int calc_iterations(double x, double y, int max_iterations) const;
  int calc_distance(double x, double y, int max_iterations) const;

  int iterate(double x, double y, int max_iterations) const;

  double distance(double x, double y, int iterations) const;

 private:
  bool   distance_;
  double d_;

  mutable double              zr_, zi_;
  mutable std::vector<double> save_x_;
  mutable std::vector<double> save_y_;
};

#endif
