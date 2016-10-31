#ifndef CQGradientPalette_H
#define CQGradientPalette_H

#include <CGradientPalette.h>
#include <QFrame>

class QPainter;

class CQGradientPalette : public QFrame {
  Q_OBJECT

 public:
  struct Margin {
    double left   = 0.1;
    double bottom = 0.1;
    double right  = 0.2;
    double top    = 0.1;
  };

 public:
  CQGradientPalette(QWidget *parent=0, CExpr *expr=0);
  CQGradientPalette(CExpr *expr=0, QWidget *parent=0);

  CGradientPalette *gradientPalette() { return pal_; }
  void setGradientPalette(CGradientPalette *pal);

  void paintEvent(QPaintEvent *);

  QSize sizeHint() const;

 private:
  void init();

  void drawAxis(QPainter *painter);

  void drawLine(QPainter *painter, double x1, double y1, double x2, double y2, const QPen &pen);

  void drawSymbol(QPainter *painter, double x, double y, const QPen &pen);

  void windowToPixel(double wx, double wy, double &px, double &py) const;
  void pixelToWindow(double px, double py, double &wx, double &wy) const;

 private:
  CExpr*            expr_ { 0 };
  CGradientPalette* pal_ { 0 };
  Margin            margin_;
};

#endif
