#include <CQGradientPaletteTest.h>
#include <CQGradientPaletteIFace.h>
#include <CQGradientPalettePlot.h>
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
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  QVBoxLayout *layout1 = new QVBoxLayout;
  layout1->setMargin(2); layout1->setSpacing(2);

  plot_    = new CQGradientPalettePlot();
  control_ = new CQGradientPaletteIFace(plot_);
  canvas_  = new Canvas(plot_);

  connect(control_, SIGNAL(stateChanged()), canvas_, SLOT(setChanged()));

  layout1->addWidget(canvas_);
  layout1->addWidget(plot_);

  layout->addLayout(layout1);
  layout->addWidget(control_);
}

CQGradientPaletteTest::
~CQGradientPaletteTest()
{
}

//------

Canvas::
Canvas(CQGradientPalettePlot *plot) :
 plot_(plot)
{
}

void
Canvas::
resizeEvent(QResizeEvent *)
{
  pixmap_  = QPixmap(width(), height());
  changed_ = true;
}

void
Canvas::
paintEvent(QPaintEvent *)
{
  if (changed_) {
    QPainter ipainter(&pixmap_);

    const double xmin = -0.66867167919799497, ymin = 0.54141012909632580;
    const double xmax = -0.39598997493734345, ymax = 0.74592140729181433;

    const int max_iterations = 256;

    ipainter.fillRect(rect(), Qt::black);

    CGradientPalette *pal = plot_->gradientPalette();

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

        ipainter.setPen(qc);

        ipainter.drawPoint(x, y);
      }
    }
  }

  //---

  QPainter painter(this);

  painter.drawPixmap(0, 0, pixmap_);

  changed_ = false;
}
