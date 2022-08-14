#ifndef CQGradientPaletteIFace_H
#define CQGradientPaletteIFace_H

#include <CGradientPalette.h>
#include <QFrame>
#include <QComboBox>
#include <QTableWidget>

class CQGradientPalettePlot;
class CQGradientPaletteColorType;
class CQGradientPaletteColorModel;
class CQGradientPaletteModel;
class CQGradientPaletteDefinedColors;
class CQRealSpin;
class QStackedWidget;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QGridLayout;

// Controls for CGradientPalette customization
class CQGradientPaletteIFace : public QFrame {
  Q_OBJECT

  Q_PROPERTY(int redModel   READ redModel   WRITE setRedModel  )
  Q_PROPERTY(int greenModel READ greenModel WRITE setGreenModel)
  Q_PROPERTY(int blueModel  READ blueModel  WRITE setBlueModel )

 public:
  CQGradientPaletteIFace(CQGradientPalettePlot *palette);

  int redModel() const;
  void setRedModel(int mode);

  int greenModel() const;
  void setGreenModel(int mode);

  int blueModel() const;
  void setBlueModel(int mode);

  std::string redFunction() const;
  void setRedFunction(const std::string &fn);

  std::string greenFunction() const;
  void setGreenFunction(const std::string &fn);

  std::string blueFunction() const;
  void setBlueFunction(const std::string &fn);

  double cubeStart() const;
  void setCubeStart(double r);

  double cubeCycles() const;
  void setCubeCycles(double r);

  double cubeSaturation() const;
  void setCubeSaturation(double r);

 signals:
  void stateChanged();

 private slots:
  void colorTypeChanged(int);
  void colorModelChanged(int);

  void modelChanged(int);

  void modelRNegativeChecked(int state);
  void modelGNegativeChecked(int state);
  void modelBNegativeChecked(int state);

  void modelRangeValueChanged(double v);

  void cubeNegativeChecked(int state);

  void colorsChanged();
  void addColorSlot();
  void loadColorsSlot();

  void functionChanged();
  void cubeValueChanged(double r);

 private:
  QFrame *createColorTypeCombo(const QString &label, CQGradientPaletteColorType **type);

  QFrame *createColorModelCombo(const QString &label, CQGradientPaletteColorModel **model);

  void createModelCombo(QGridLayout *grid, int row, const QString &label,
                        CQGradientPaletteModel **model);

  void createFunctionEdit(QGridLayout *grid, int row, const QString &label, QLineEdit **edit);

  void createRealEdit(QGridLayout *grid, int row, const QString &label, CQRealSpin **edit);

 private:
  CQGradientPalettePlot*          palette_             { nullptr };
  CQGradientPaletteColorType*     colorType_           { nullptr };
  CQGradientPaletteColorModel*    colorModel_          { nullptr };
  QStackedWidget*                 stack_               { nullptr };
  CQGradientPaletteModel*         redModel_            { nullptr };
  CQGradientPaletteModel*         greenModel_          { nullptr };
  CQGradientPaletteModel*         blueModel_           { nullptr };
  QCheckBox*                      modelRNegativeCheck_ { nullptr };
  QCheckBox*                      modelGNegativeCheck_ { nullptr };
  QCheckBox*                      modelBNegativeCheck_ { nullptr };
  CQRealSpin*                     redMin_              { nullptr };
  CQRealSpin*                     redMax_              { nullptr };
  CQRealSpin*                     greenMin_            { nullptr };
  CQRealSpin*                     greenMax_            { nullptr };
  CQRealSpin*                     blueMin_             { nullptr };
  CQRealSpin*                     blueMax_             { nullptr };
  QLineEdit*                      redFunction_         { nullptr };
  QLineEdit*                      greenFunction_       { nullptr };
  QLineEdit*                      blueFunction_        { nullptr };
  CQGradientPaletteDefinedColors* definedColors_       { nullptr };
  QPushButton*                    addColorButton_      { nullptr };
  QPushButton*                    loadColorsButton_    { nullptr };
  CQRealSpin*                     cubeStart_           { nullptr };
  CQRealSpin*                     cubeCycles_          { nullptr };
  CQRealSpin*                     cubeSaturation_      { nullptr };
  QCheckBox*                      cubeNegativeCheck_   { nullptr };
};

//---

class CQGradientPaletteColorType : public QComboBox {
  Q_OBJECT

 public:
  CQGradientPaletteColorType(QWidget *parent=0);

  CGradientPalette::ColorType type() const;

  void setType(const CGradientPalette::ColorType &type);
};

//---

class CQGradientPaletteColorModel : public QComboBox {
  Q_OBJECT

 public:
  CQGradientPaletteColorModel(QWidget *parent=0);

  CGradientPalette::ColorModel model() const;

  void setModel(const CGradientPalette::ColorModel &model);
};

//---

class CQGradientPaletteModel : public QComboBox {
  Q_OBJECT

 public:
  CQGradientPaletteModel(QWidget *parent=0);
};

//---

class CQGradientPaletteDefinedColors : public QTableWidget {
  Q_OBJECT

 public:
  struct RealColor {
    RealColor(double r1, const CRGBA &c1) :
     r(r1), c(c1) {
    }

    double r { 0.0 };
    CRGBA  c { 0, 0, 0 };
  };

  typedef std::vector<RealColor> RealColors;

 public:
  CQGradientPaletteDefinedColors(QWidget *parent=0);

  void updateColors(CGradientPalette *palette);

  uint numRealColors() const { return uint(realColors_.size()); }

  const RealColor &realColor(int i) const;

  void setRealColor(int i, const RealColor &c);

 signals:
  void colorsChanged();

 private:
  RealColors realColors_;
};

#endif
