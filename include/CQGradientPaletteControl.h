#ifndef CQGradientPaletteControl_H
#define CQGradientPaletteControl_H

#include <CGradientPalette.h>
#include <QFrame>
#include <QComboBox>
#include <QTableWidget>

class CQGradientPalette;
class CQGradientPaletteColorType;
class CQGradientPaletteModel;
class CQGradientPaletteDefinedColors;
class CQRealSpin;
class QStackedWidget;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QGridLayout;

class CQGradientPaletteControl : public QFrame {
  Q_OBJECT

  Q_PROPERTY(int redModel   READ redModel   WRITE setRedModel  )
  Q_PROPERTY(int greenModel READ greenModel WRITE setGreenModel)
  Q_PROPERTY(int blueModel  READ blueModel  WRITE setBlueModel )

 public:
  CQGradientPaletteControl(CQGradientPalette *palette);

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

  void modelChanged(int);

  void rNegativeChecked(int state);
  void gNegativeChecked(int state);
  void bNegativeChecked(int state);

  void cubeNegativeChecked(int state);

  void colorsChanged();
  void addColorSlot();
  void loadColorsSlot();

  void functionChanged();
  void cubeValueChanged(double r);

 private:
  QFrame *createColorTypeCombo(const QString &label, CQGradientPaletteColorType **type);

  void createModelCombo(QGridLayout *grid, int row, const QString &label,
                        CQGradientPaletteModel **model);

  void createFunctionEdit(QGridLayout *grid, int row, const QString &label, QLineEdit **edit);

  void createRealEdit(QGridLayout *grid, int row, const QString &label, CQRealSpin **edit);

 private:
  CQGradientPalette*              palette_             { nullptr };
  CQGradientPaletteColorType*     colorType_           { nullptr };
  QStackedWidget*                 stack_               { nullptr };
  CQGradientPaletteModel*         redModel_            { nullptr };
  CQGradientPaletteModel*         greenModel_          { nullptr };
  CQGradientPaletteModel*         blueModel_           { nullptr };
  QCheckBox*                      modelRNegativeCheck_ { nullptr };
  QCheckBox*                      modelGNegativeCheck_ { nullptr };
  QCheckBox*                      modelBNegativeCheck_ { nullptr };
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

  int numRealColors() const { return realColors_.size(); }

  const RealColor &realColor(int i) const;

  void setRealColor(int i, const RealColor &c);

 signals:
  void colorsChanged();

 private:
  RealColors realColors_;
};

#endif
