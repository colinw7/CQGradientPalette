#include <QDialog>
#include <QFrame>
#include <CMandelbrot.h>

class CQGradientPalettePlot;
class CQGradientPaletteIFace;
class Canvas;

class CQGradientPaletteTest : public QDialog {
  Q_OBJECT

 public:
  CQGradientPaletteTest();
 ~CQGradientPaletteTest();

 private:
  CQGradientPalettePlot  *plot_    { nullptr };
  CQGradientPaletteIFace *control_ { nullptr };
  Canvas                 *canvas_  { nullptr };
};

//---

class Canvas : public QFrame {
  Q_OBJECT

 public:
  Canvas(CQGradientPalettePlot *plot);

  void resizeEvent(QResizeEvent *) override;

  void paintEvent(QPaintEvent *) override;

  QSize sizeHint() const { return QSize(400, 400); }

 public slots:
  void setChanged() { changed_ = true; update(); }

 private:
  CQGradientPalettePlot *plot_    { nullptr };
  CMandelbrot            mandelbrot_;
  QPixmap                pixmap_;
  bool                   changed_ { true };
};
