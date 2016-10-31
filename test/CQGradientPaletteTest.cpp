#include <CQGradientPaletteTest.h>
#include <CQGradientPalette.h>
#include <CQGradientPaletteControl.h>
#include <QApplication>
#include <QVBoxLayout>
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

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CQGradientPaletteTest *test = new CQGradientPaletteTest;

  test->show();

  return app.exec();
}

CQGradientPaletteTest::
CQGradientPaletteTest()
{
  CExpr *expr = new CExpr;

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  QVBoxLayout *layout1 = new QVBoxLayout;
  layout1->setMargin(2); layout1->setSpacing(2);

  palette_ = new CQGradientPalette(expr);
  control_ = new CQGradientPaletteControl(palette_);
  canvas_  = new Canvas(palette_);

  connect(control_, SIGNAL(stateChanged()), canvas_, SLOT(update()));

  layout1->addWidget(canvas_);
  layout1->addWidget(palette_);

  layout->addLayout(layout1);
  layout->addWidget(control_);
}

Canvas::
Canvas(CQGradientPalette *palette) :
 palette_(palette)
{
}

void
Canvas::
paintEvent(QPaintEvent *)
{
  double xmin = -0.66867167919799497, ymin = 0.54141012909632580;
  double xmax = -0.39598997493734345, ymax = 0.74592140729181433;

  static int max_iterations = 256;

  QPainter painter(this);

  painter.fillRect(rect(), Qt::black);

  CGradientPalette *pal = palette_->gradientPalette();

  mandelbrot_.initCalc(0, 0, width() - 1, height() - 1);

  for (int y = 0; y < height(); ++y) {
    double wy = Util::map(y, height() - 1, 0, ymin, ymax);

    for (int x = 0; x < width(); ++x) {
      double wx = Util::map(x, 0, width () - 1, xmin, xmax);

      int i = mandelbrot_.iterate(wx, wy, max_iterations);

      if (i >= max_iterations)
        continue;

      double z = (1.0*i)/max_iterations;

      CRGBA c = pal->getColor(z).rgba();

      QColor qc(c.getRedI(), c.getGreenI(), c.getBlueI());

      painter.setPen(qc);

      painter.drawPoint(x, y);
    }
  }
}
