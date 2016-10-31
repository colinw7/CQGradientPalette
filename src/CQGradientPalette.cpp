#include <CQGradientPalette.h>
#include <QPainter>

namespace Util {
  inline double norm(double x, double low, double high) {
    return (x - low)/(high - low);
  }

  inline double lerp(double value1, double value2, double amt) {
    return value1 + (value2 - value1)*amt;
  }

  inline double map(double value, double low1, double high1, double low2, double high2) {
    return lerp(low2, high2, norm(value, low1, high1));
  }
}

//------

CQGradientPalette::
CQGradientPalette(QWidget *parent, CExpr *expr) :
 QFrame(parent), expr_(expr), pal_(0)
{
  init();
}

CQGradientPalette::
CQGradientPalette(CExpr *expr, QWidget *parent) :
 QFrame(parent), expr_(expr), pal_(0)
{
  init();
}

void
CQGradientPalette::
init()
{
  if (! pal_)
    pal_ = new CGradientPalette(expr_);

  pal_->addDefinedColor(0, CRGBA(0,0,0));
  pal_->addDefinedColor(1, CRGBA(1,1,1));
}

void
CQGradientPalette::
setGradientPalette(CGradientPalette *pal)
{
  delete pal_;

  pal_ = pal;
}

void
CQGradientPalette::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.fillRect(rect(), Qt::white);

  //---

  drawAxis(&painter);

  //---

  // draw graph
  QPen redPen  (Qt::red  ); redPen  .setWidth(0);
  QPen greenPen(Qt::green); greenPen.setWidth(0);
  QPen bluePen (Qt::blue ); bluePen .setWidth(0);
  QPen blackPen(Qt::black); blackPen.setWidth(0);

  double px1, py1, px2, py2;

  windowToPixel(0.0, 0.0, px1, py1);
  windowToPixel(1.0, 1.0, px2, py2);

  bool   first = true;
  double r1, g1, b1, m1, x1;

  for (double x = px1; x <= px2; x += 1.0) {
    double wx, wy;

    pixelToWindow(x, 0, wx, wy);

    CColor c = pal_->getColor(wx);

    CRGBA rgba = c.rgba();

    double x2 = wx;
    double r2 = rgba.getRed  ();
    double g2 = rgba.getGreen();
    double b2 = rgba.getBlue ();
    double m2 = rgba.getGray();

    if (! first) {
      drawLine(&painter, x1, r1, x2, r2, redPen  );
      drawLine(&painter, x1, g1, x2, g2, greenPen);
      drawLine(&painter, x1, b1, x2, b2, bluePen );
      drawLine(&painter, x1, m1, x2, m2, blackPen);
    }

    x1 = x2;
    r1 = r2;
    g1 = g2;
    b1 = b2;
    m1 = m2;

    first = false;
  }

  //---

  // draw color bar
  double xp1 = 1.05;
  double xp2 = 1.15;

  double pxp1, pxp2;

  windowToPixel(xp1, 0.0, pxp1, py1);
  windowToPixel(xp2, 1.0, pxp2, py2);

  for (double y = py2; y <= py1; y += 1.0) {
    double wx, wy;

    pixelToWindow(0, y, wx, wy);

    CColor c = pal_->getColor(wy);

    CRGBA rgba = c.rgba();

    QColor qc(rgba.getRedI(), rgba.getGreenI(), rgba.getBlueI());

    QPen pen(qc); pen.setWidth(0);

    painter.setPen(pen);

    painter.drawLine(QPointF(pxp1, y), QPointF(pxp2, y));
  }

  painter.setPen(blackPen);

  painter.drawLine(QPointF(pxp1, py1), QPointF(pxp2, py1));
  painter.drawLine(QPointF(pxp2, py1), QPointF(pxp2, py2));
  painter.drawLine(QPointF(pxp2, py2), QPointF(pxp1, py2));
  painter.drawLine(QPointF(pxp1, py2), QPointF(pxp1, py1));

  //---

  if (pal_->colorType() == CGradientPalette::ColorType::DEFINED) {
    for (const auto &c : pal_->colors()) {
      double x    = c.first;
      CRGBA  rgba = c.second.rgba();

      drawSymbol(&painter, x, rgba.getRed  (), redPen  );
      drawSymbol(&painter, x, rgba.getGreen(), greenPen);
      drawSymbol(&painter, x, rgba.getBlue (), bluePen );
    }
  }
}

void
CQGradientPalette::
drawAxis(QPainter *painter)
{
  QPen blackPen(Qt::black); blackPen.setWidth(0);

  double px1, py1, px2, py2;

  windowToPixel(0.0, 0.0, px1, py1);
  windowToPixel(1.0, 1.0, px2, py2);

  drawLine(painter, 0, 0, 1, 0, blackPen);
  drawLine(painter, 0, 0, 0, 1, blackPen);

  painter->setPen(blackPen);

  painter->drawLine(QPointF(px1, py1), QPointF(px1, py1 + 4));
  painter->drawLine(QPointF(px2, py1), QPointF(px2, py1 + 4));

  painter->drawLine(QPointF(px1, py1), QPointF(px1 - 4, py1));
  painter->drawLine(QPointF(px1, py2), QPointF(px1 - 4, py2));
}

void
CQGradientPalette::
drawLine(QPainter *painter, double x1, double y1, double x2, double y2, const QPen &pen)
{
  painter->setPen(pen);

  double px1, py1, px2, py2;

  windowToPixel(x1, y1, px1, py1);
  windowToPixel(x2, y2, px2, py2);

  painter->drawLine(QPointF(px1, py1), QPointF(px2, py2));
}

void
CQGradientPalette::
drawSymbol(QPainter *painter, double x, double y, const QPen &pen)
{
  painter->setPen(pen);

  double px, py;

  windowToPixel(x, y, px, py);

  painter->drawLine(QPointF(px - 4, py), QPointF(px + 4, py));
  painter->drawLine(QPointF(px, py - 4), QPointF(px, py + 4));
}

void
CQGradientPalette::
windowToPixel(double wx, double wy, double &px, double &py) const
{
  px = Util::map(wx, -margin_.left, 1 + margin_.right, 0, width () - 1);
  py = Util::map(wy, -margin_.bottom, 1 + margin_.top, height() - 1, 0);
}

void
CQGradientPalette::
pixelToWindow(double px, double py, double &wx, double &wy) const
{
  wx = Util::map(px, 0, width () - 1, -margin_.left, 1 + margin_.right);
  wy = Util::map(py, height() - 1, 0, -margin_.bottom, 1 + margin_.top);
}

QSize
CQGradientPalette::
sizeHint() const
{
  return QSize(600, 600);
}
