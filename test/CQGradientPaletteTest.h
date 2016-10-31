#include <QDialog>
#include <QFrame>
#include <CMandelbrot.h>

class CQGradientPalette;
class CQGradientPaletteControl;
class Canvas;

class CQGradientPaletteTest : public QDialog {
  Q_OBJECT

 public:
  CQGradientPaletteTest();

 private:
  CQGradientPalette        *palette_;
  CQGradientPaletteControl *control_;
  Canvas                   *canvas_;
};

class Canvas : public QFrame {
  Q_OBJECT

 public:
  Canvas(CQGradientPalette *palette);

  void paintEvent(QPaintEvent *);

  QSize sizeHint() const { return QSize(400, 400); }

 private:
  CQGradientPalette *palette_;
  CMandelbrot        mandelbrot_;
};
