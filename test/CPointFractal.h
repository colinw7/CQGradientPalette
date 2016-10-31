#ifndef CPOINT_FRACTAL_H
#define CPOINT_FRACTAL_H

#include <CRGBA.h>
#include <CColorRange.h>
#include <CTransform2D.h>

class CPointFractalCalc {
 public:
  CPointFractalCalc() :
   a_(0), show_vector_(false) {
  }

  virtual ~CPointFractalCalc() { }

  virtual CPointFractalCalc *dup() const = 0;

  void setAngle(double angle) { a_ = angle; }

  double getAngle() const { return a_; }

  virtual double getXMin() const = 0;
  virtual double getYMin() const = 0;
  virtual double getXMax() const = 0;
  virtual double getYMax() const = 0;

  bool getShowVector() const { return show_vector_; }

  void setShowVector(bool show_vector) { show_vector_ = show_vector; }

  virtual void setRange(double, double, double, double) { }

  virtual void initCalc(int, int, int, int, double, double, double, double, int) { }

  virtual int calc(double x, double y, int max_iterations) const = 0;

 protected:
  double a_;
  bool   show_vector_;
};

class CPointFractal {
 public:
  CPointFractal(CPointFractalCalc *calc);

  virtual ~CPointFractal();

  bool getShowVector() const {
    return calc_->getShowVector();
  }

  void setShowVector(bool show_vector) {
    calc_->setShowVector(show_vector);
  }

  void conv(double px, double py, double *wx, double *wy);

  void iconv(double wx, double wy, double *px, double *py);

  void draw(int pixel_xmin, int pixel_ymin, int pixel_xmax, int pixel_ymax,
            double xmin, double ymin, double xmax, double ymax, int max_iterations);

  void threadDraw(int pixel_xmin, int pixel_ymin, int pixel_xmax, int pixel_ymax,
                  double xmin, double ymin, double xmax, double ymax, int max_iterations,
                  uint nx, uint ny);

  virtual void setForeground(const CRGBA &rgba) = 0;

  virtual void drawPoint(int x, int y) = 0;

 protected:
  CPointFractalCalc *calc_;
  CColorRange        colors_;
  CTransform2D       transform_;
  double             s_, c_;

  std::vector<double> x_store_;
  std::vector<double> y_store_;
};

#endif
