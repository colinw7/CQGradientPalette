#ifndef CQGradientPalettePlot_H
#define CQGradientPalettePlot_H

#include <CGradientPalette.h>
#include <QFrame>

class QPainter;

class CQGradientPalettePlot : public QFrame {
  Q_OBJECT

 public:
  struct Margin {
    double left   = 0.1;
    double bottom = 0.1;
    double right  = 0.2;
    double top    = 0.1;
  };

 public:
  CQGradientPalettePlot(QWidget *parent=nullptr, CExpr *expr=nullptr);
  CQGradientPalettePlot(CExpr *expr, QWidget *parent=nullptr);

 ~CQGradientPalettePlot();

  CGradientPalette *gradientPalette() { return pal_; }
  void setGradientPalette(CGradientPalette *pal);

  void paintEvent(QPaintEvent *) override;

  QSize sizeHint() const override;

 private:
  void init();

  void drawAxis(QPainter *painter);

  void drawLine(QPainter *painter, double x1, double y1, double x2, double y2, const QPen &pen);

  void drawSymbol(QPainter *painter, double x, double y, const QPen &pen);

  void windowToPixel(double wx, double wy, double &px, double &py) const;
  void pixelToWindow(double px, double py, double &wx, double &wy) const;

 private:
  CExpr*            expr_ { nullptr };
  CGradientPalette* pal_  { nullptr };
  Margin            margin_;
};

#endif
