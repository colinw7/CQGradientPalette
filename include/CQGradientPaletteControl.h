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
class QGridLayout;
class QStackedWidget;
class QLineEdit;
class QPushButton;
class CQRealSpin;

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
  CQGradientPalette*              palette_;
  CQGradientPaletteColorType*     colorType_;
  QStackedWidget*                 stack_;
  CQGradientPaletteModel*         redModel_;
  CQGradientPaletteModel*         greenModel_;
  CQGradientPaletteModel*         blueModel_;
  QLineEdit*                      redFunction_;
  QLineEdit*                      greenFunction_;
  QLineEdit*                      blueFunction_;
  CQGradientPaletteDefinedColors* definedColors_;
  QPushButton*                    addColorButton_;
  QPushButton*                    loadColorsButton_;
  CQRealSpin*                     cubeStart_;
  CQRealSpin*                     cubeCycles_;
  CQRealSpin*                     cubeSaturation_;
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

    double r;
    CRGBA  c;
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
