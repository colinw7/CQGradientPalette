#include <CQGradientPaletteControl.h>
#include <CQGradientPalette.h>
#include <CQRealSpin.h>
#include <CQColorChooser.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QHeaderView>
#include <QItemDelegate>

CQGradientPaletteControl::
CQGradientPaletteControl(CQGradientPalette *palette) :
 QFrame(0), palette_(palette)
{
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  CGradientPalette *pal = palette_->gradientPalette();

  QFrame *typeFrame = createColorTypeCombo("Type", &colorType_);

  colorType_->setType(pal->colorType());

  connect(colorType_, SIGNAL(currentIndexChanged(int)), this, SLOT(colorTypeChanged(int)));

  layout->addWidget(typeFrame);

  //---

  stack_ = new QStackedWidget;

  //---

  QFrame *modelFrame = new QFrame;

  QGridLayout *modelGridLayout = new QGridLayout(modelFrame);
  modelGridLayout->setMargin(2); modelGridLayout->setSpacing(2);

  createModelCombo(modelGridLayout, 0, "Red"  , &redModel_  );
  createModelCombo(modelGridLayout, 1, "Green", &greenModel_);
  createModelCombo(modelGridLayout, 2, "Blue" , &blueModel_ );

  modelGridLayout->setRowStretch(3, 1);

  setRedModel  (pal->redModel  ());
  setGreenModel(pal->greenModel());
  setBlueModel (pal->blueModel ());

  connect(redModel_  , SIGNAL(currentIndexChanged(int)), this, SLOT(modelChanged(int)));
  connect(greenModel_, SIGNAL(currentIndexChanged(int)), this, SLOT(modelChanged(int)));
  connect(blueModel_ , SIGNAL(currentIndexChanged(int)), this, SLOT(modelChanged(int)));

  stack_->addWidget(modelFrame);

  //---

  QFrame *definedFrame = new QFrame;

  QVBoxLayout *definedFrameLayout = new QVBoxLayout(definedFrame);
  definedFrameLayout->setMargin(2); definedFrameLayout->setSpacing(2);

  definedColors_ = new CQGradientPaletteDefinedColors;

  definedColors_->updateColors(pal);

  definedFrameLayout->addWidget(definedColors_);

  connect(definedColors_, SIGNAL(colorsChanged()), this, SLOT(colorsChanged()));

  QFrame *definedButtonsFrame = new QFrame;

  QHBoxLayout *definedButtonsLayout = new QHBoxLayout(definedButtonsFrame);
  definedButtonsLayout->setMargin(2); definedButtonsLayout->setSpacing(2);

  addColorButton_   = new QPushButton("Add");
  loadColorsButton_ = new QPushButton("Load");

  definedButtonsLayout->addWidget(addColorButton_);
  definedButtonsLayout->addWidget(loadColorsButton_);
  definedButtonsLayout->addStretch(1);

  connect(addColorButton_, SIGNAL(clicked()), this, SLOT(addColorSlot()));
  connect(loadColorsButton_, SIGNAL(clicked()), this, SLOT(loadColorsSlot()));

  definedFrameLayout->addWidget(definedButtonsFrame);

  stack_->addWidget(definedFrame);

  //---

  QFrame *functionsFrame = new QFrame;

  QGridLayout *functionsGridLayout = new QGridLayout(functionsFrame);
  functionsGridLayout->setMargin(2); functionsGridLayout->setSpacing(2);

  createFunctionEdit(functionsGridLayout, 0, "Red"  , &redFunction_  );
  createFunctionEdit(functionsGridLayout, 1, "Green", &greenFunction_);
  createFunctionEdit(functionsGridLayout, 2, "Blue" , &blueFunction_ );

  functionsGridLayout->setRowStretch(3, 1);

  setRedFunction  (pal->redFunction  ());
  setGreenFunction(pal->greenFunction());
  setBlueFunction (pal->blueFunction ());

  connect(redFunction_  , SIGNAL(editingFinished()), this, SLOT(functionChanged()));
  connect(greenFunction_, SIGNAL(editingFinished()), this, SLOT(functionChanged()));
  connect(blueFunction_ , SIGNAL(editingFinished()), this, SLOT(functionChanged()));

  stack_->addWidget(functionsFrame);

  //---

  QFrame *cubeFrame = new QFrame;

  QGridLayout *cubeGridLayout = new QGridLayout(cubeFrame);
  cubeGridLayout->setMargin(2); cubeGridLayout->setSpacing(2);

  createRealEdit(cubeGridLayout, 0, "Start"     , &cubeStart_     );
  createRealEdit(cubeGridLayout, 1, "Cycles"    , &cubeCycles_    );
  createRealEdit(cubeGridLayout, 2, "Saturation", &cubeSaturation_);

  cubeGridLayout->setRowStretch(3, 1);

  setCubeStart     (pal->cbStart     ());
  setCubeCycles    (pal->cbCycles    ());
  setCubeSaturation(pal->cbSaturation());

  connect(cubeStart_     , SIGNAL(valueChanged(double)), this, SLOT(cubeValueChanged(double)));
  connect(cubeCycles_    , SIGNAL(valueChanged(double)), this, SLOT(cubeValueChanged(double)));
  connect(cubeSaturation_, SIGNAL(valueChanged(double)), this, SLOT(cubeValueChanged(double)));

  stack_->addWidget(cubeFrame);

  //---

  layout->addWidget(stack_);
  layout->addStretch(1);

  //---

  connect(this, SIGNAL(stateChanged()), palette, SLOT(update()));
}

void
CQGradientPaletteControl::
colorTypeChanged(int)
{
  CGradientPalette *pal = palette_->gradientPalette();

  pal->setColorType(colorType_->type());

  if      (pal->colorType() == CGradientPalette::ColorType::MODEL)
    stack_->setCurrentIndex(0);
  else if (pal->colorType() == CGradientPalette::ColorType::DEFINED)
    stack_->setCurrentIndex(1);
  else if (pal->colorType() == CGradientPalette::ColorType::FUNCTIONS)
    stack_->setCurrentIndex(2);
  else if (pal->colorType() == CGradientPalette::ColorType::CUBEHELIX)
    stack_->setCurrentIndex(3);

  emit stateChanged();
}

int
CQGradientPaletteControl::
redModel() const
{
  return redModel_->currentIndex();
}

void
CQGradientPaletteControl::
setRedModel(int model)
{
  redModel_->setCurrentIndex(model);
}

int
CQGradientPaletteControl::
greenModel() const
{
  return greenModel_->currentIndex();
}

void
CQGradientPaletteControl::
setGreenModel(int model)
{
  greenModel_->setCurrentIndex(model);
}

int
CQGradientPaletteControl::
blueModel() const
{
  return blueModel_->currentIndex();
}

void
CQGradientPaletteControl::
setBlueModel(int model)
{
  blueModel_->setCurrentIndex(model);
}

void
CQGradientPaletteControl::
modelChanged(int model)
{
  CQGradientPaletteModel *me = qobject_cast<CQGradientPaletteModel *>(sender());
  assert(me);

  CGradientPalette *pal = palette_->gradientPalette();

  if      (me == redModel_)
    pal->setRedModel(model);
  else if (me == greenModel_)
    pal->setGreenModel(model);
  else if (me == blueModel_)
    pal->setBlueModel(model);

  emit stateChanged();
}

void
CQGradientPaletteControl::
colorsChanged()
{
  CGradientPalette *pal = palette_->gradientPalette();

  pal->resetDefinedColors();

  for (int i = 0; i < definedColors_->numRealColors(); ++i) {
    const CQGradientPaletteDefinedColors::RealColor &realColor = definedColors_->realColor(i);

    pal->addDefinedColor(realColor.r, realColor.c);
  }

  emit stateChanged();
}

void
CQGradientPaletteControl::
addColorSlot()
{
  CGradientPalette *pal = palette_->gradientPalette();

  pal->addDefinedColor(0.5, CRGBA(0.5, 0.5, 0.5));

  definedColors_->updateColors(pal);

  emit stateChanged();
}

void
CQGradientPaletteControl::
loadColorsSlot()
{
  CGradientPalette *pal = palette_->gradientPalette();

  QString dir = QDir::current().dirName();

  QString fileName = QFileDialog::getOpenFileName(this, "Open File", dir, "Files (*.*)");

  if (! fileName.length())
    return;

  pal->readFile(fileName.toStdString());

  definedColors_->updateColors(pal);
}

std::string
CQGradientPaletteControl::
redFunction() const
{
  return redFunction_->text().toStdString();
}

void
CQGradientPaletteControl::
setRedFunction(const std::string &fn)
{
  redFunction_->setText(fn.c_str());
}

std::string
CQGradientPaletteControl::
greenFunction() const
{
  return greenFunction_->text().toStdString();
}

void
CQGradientPaletteControl::
setGreenFunction(const std::string &fn)
{
  greenFunction_->setText(fn.c_str());
}

std::string
CQGradientPaletteControl::
blueFunction() const
{
  return blueFunction_->text().toStdString();
}

void
CQGradientPaletteControl::
setBlueFunction(const std::string &fn)
{
  blueFunction_->setText(fn.c_str());
}

void
CQGradientPaletteControl::
functionChanged()
{
  QLineEdit *le = qobject_cast<QLineEdit *>(sender());
  assert(le);

  CGradientPalette *pal = palette_->gradientPalette();

  if      (le == redFunction_)
    pal->setRedFunction  (le->text().toStdString());
  else if (le == greenFunction_)
    pal->setGreenFunction(le->text().toStdString());
  else if (le == blueFunction_)
    pal->setBlueFunction (le->text().toStdString());

  emit stateChanged();
}

double
CQGradientPaletteControl::
cubeStart() const
{
  return cubeStart_->value();
}

void
CQGradientPaletteControl::
setCubeStart(double r)
{
  cubeStart_->setValue(r);
}

double
CQGradientPaletteControl::
cubeCycles() const
{
  return cubeCycles_->value();
}

void
CQGradientPaletteControl::
setCubeCycles(double r)
{
  cubeCycles_->setValue(r);
}

double
CQGradientPaletteControl::
cubeSaturation() const
{
  return cubeSaturation_->value();
}

void
CQGradientPaletteControl::
setCubeSaturation(double r)
{
  cubeSaturation_->setValue(r);
}

void
CQGradientPaletteControl::
cubeValueChanged(double r)
{
  CQRealSpin *rs = qobject_cast<CQRealSpin *>(sender());
  assert(rs);

  CGradientPalette *pal = palette_->gradientPalette();

  if      (rs == cubeStart_)
    pal->setCbStart(r);
  else if (rs == cubeCycles_)
    pal->setCbCycles(r);
  else if (rs == cubeSaturation_)
    pal->setCbSaturation(r);

  emit stateChanged();
}

QFrame *
CQGradientPaletteControl::
createColorTypeCombo(const QString &label, CQGradientPaletteColorType **type)
{
  QFrame *frame = new QFrame;

  QHBoxLayout *layout = new QHBoxLayout(frame);
  layout->setMargin(2); layout->setSpacing(2);

  *type = new CQGradientPaletteColorType;

  layout->addWidget(new QLabel(label));
  layout->addWidget(*type);
  layout->addStretch(1);

  return frame;
}

void
CQGradientPaletteControl::
createModelCombo(QGridLayout *grid, int row, const QString &label, CQGradientPaletteModel **model)
{
  *model = new CQGradientPaletteModel;

  grid->addWidget(new QLabel(label), row, 0);
  grid->addWidget(*model, row, 1);

  grid->setColumnStretch(2, true);
}

void
CQGradientPaletteControl::
createFunctionEdit(QGridLayout *grid, int row, const QString &label, QLineEdit **edit)
{
  *edit = new QLineEdit;

  grid->addWidget(new QLabel(label), row, 0);
  grid->addWidget(*edit, row, 1);

  grid->setColumnStretch(2, true);
}

void
CQGradientPaletteControl::
createRealEdit(QGridLayout *grid, int row, const QString &label, CQRealSpin **edit)
{
  *edit = new CQRealSpin;

  grid->addWidget(new QLabel(label), row, 0);
  grid->addWidget(*edit, row, 1);

  grid->setColumnStretch(2, true);
}

//---

CQGradientPaletteColorType::
CQGradientPaletteColorType(QWidget *parent) :
 QComboBox(parent)
{
  addItem("Model"     , QVariant(static_cast<int>(CGradientPalette::ColorType::MODEL    )));
  addItem("Defined"   , QVariant(static_cast<int>(CGradientPalette::ColorType::DEFINED  )));
  addItem("Functions" , QVariant(static_cast<int>(CGradientPalette::ColorType::FUNCTIONS)));
  addItem("Cube Helix", QVariant(static_cast<int>(CGradientPalette::ColorType::CUBEHELIX)));
}

CGradientPalette::ColorType
CQGradientPaletteColorType::
type() const
{
  return static_cast<CGradientPalette::ColorType>(itemData(currentIndex()).toInt());
}

void
CQGradientPaletteColorType::
setType(const CGradientPalette::ColorType &type)
{
  QVariant var(static_cast<int>(type));

  for (int i = 0; i < count(); ++i)
    if (itemData(i).toInt() == var.toInt())
      setCurrentIndex(i);
}

//---

CQGradientPaletteModel::
CQGradientPaletteModel(QWidget *parent) :
 QComboBox(parent)
{
  for (int i = 0; i < CGradientPalette::numModels(); ++i)
    addItem(CGradientPalette::modelName(i).c_str());
}

//---

class CQGradientPaletteDefinedColorsDelegate : public QItemDelegate {
 public:
  CQGradientPaletteDefinedColorsDelegate(CQGradientPaletteDefinedColors *colors_);

  // Override to create editor
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;

  // Override to get content from editor
  void setEditorData(QWidget *editor, const QModelIndex &index) const;

  // Override to set editor from content
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;

 private:
  CQGradientPaletteDefinedColors *colors_;
};

CQGradientPaletteDefinedColors::
CQGradientPaletteDefinedColors(QWidget *parent) :
 QTableWidget(parent)
{
  verticalHeader()->hide();

  CQGradientPaletteDefinedColorsDelegate *delegate =
    new CQGradientPaletteDefinedColorsDelegate(this);

  setItemDelegate(delegate);
  setEditTriggers(QAbstractItemView::AllEditTriggers);

  setSelectionMode(QAbstractItemView::SingleSelection);
  setSelectionBehavior(QAbstractItemView::SelectRows);

  QHeaderView *header = horizontalHeader();

  header->setStretchLastSection(true) ;
}

void
CQGradientPaletteDefinedColors::
updateColors(CGradientPalette *palette)
{
  setColumnCount(2);

  setHorizontalHeaderLabels(QStringList() << "X" << "Color");

  setRowCount(palette->numColors());

  realColors_.clear();

  for (const auto &c : palette->colors())
    realColors_.emplace_back(c.first, c.second.rgba());

  for (int r = 0; r < numRealColors(); ++r) {
    const RealColor &realColor = this->realColor(r);

    QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1").arg(realColor.r));
    QTableWidgetItem *item2 = new QTableWidgetItem(realColor.c.getRGB().toString().c_str());

    setItem(r, 0, item1);
    setItem(r, 1, item2);
  }
}

const CQGradientPaletteDefinedColors::RealColor &
CQGradientPaletteDefinedColors::
realColor(int r) const
{
  return realColors_[r];
}

void
CQGradientPaletteDefinedColors::
setRealColor(int r, const RealColor &realColor)
{
  realColors_[r] = realColor;

  //QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1").arg(realColor.r));
  //QTableWidgetItem *item2 = new QTableWidgetItem(realColor.c.getRGB().toString().c_str());

  //setItem(r, 0, item1);
  //setItem(r, 1, item2);

  emit colorsChanged();
}

//---

CQGradientPaletteDefinedColorsDelegate::
CQGradientPaletteDefinedColorsDelegate(CQGradientPaletteDefinedColors *colors_) :
 QItemDelegate(colors_), colors_(colors_)
{
}

QWidget *
CQGradientPaletteDefinedColorsDelegate::
createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTableWidgetItem *item = colors_->item(index.row(), index.column());
  assert(item);

  const CQGradientPaletteDefinedColors::RealColor &realColor = colors_->realColor(index.row());

  if       (index.column() == 0) {
    CQRealSpin *edit = new CQRealSpin(parent);

    edit->setValue(realColor.r);

    return edit;
  }
  else if (index.column() == 1) {
    CQColorChooser *edit = new CQColorChooser(parent);

    edit->setStyles(CQColorChooser::Text | CQColorChooser::ColorButton);

    edit->setColor(QColor(realColor.c.getRedI(), realColor.c.getGreenI(), realColor.c.getBlueI()));

    return edit;
  }
  else
    return QItemDelegate::createEditor(parent, option, index);
}

// model->editor
void
CQGradientPaletteDefinedColorsDelegate::
setEditorData(QWidget *w, const QModelIndex &index) const
{
  QTableWidgetItem *item = colors_->item(index.row(), index.column());
  assert(item);

  const CQGradientPaletteDefinedColors::RealColor &realColor = colors_->realColor(index.row());

  if       (index.column() == 0) {
    CQRealSpin *edit = qobject_cast<CQRealSpin *>(w);

    edit->setValue(realColor.r);
  }
  else if (index.column() == 1) {
    CQColorChooser *edit = qobject_cast<CQColorChooser *>(w);

    const CRGBA &c = realColor.c;

    QColor qc(c.getRedI(), c.getGreenI(), c.getBlueI());

    edit->setColor(qc);
  }
  else
    QItemDelegate::setEditorData(w, index);
}

// editor->model
void
CQGradientPaletteDefinedColorsDelegate::
setModelData(QWidget *w, QAbstractItemModel *model, const QModelIndex &index) const
{
  QTableWidgetItem *item = colors_->item(index.row(), index.column());
  assert(item);

  CQGradientPaletteDefinedColors::RealColor realColor = colors_->realColor(index.row());

  if       (index.column() == 0) {
    CQRealSpin *edit = qobject_cast<CQRealSpin *>(w);

    double r = edit->value();

    model->setData(index, QVariant(r));

    realColor.r = r;
  }
  else if (index.column() == 1) {
    CQColorChooser *edit = qobject_cast<CQColorChooser *>(w);

    QColor qc = edit->color();

    realColor.c = CRGBA(qc.redF(), qc.greenF(), qc.blueF(), qc.alphaF());

    model->setData(index, QVariant(qc));
  }
  else
    QItemDelegate::setModelData(w, model, index);

  colors_->setRealColor(index.row(), realColor);
}

QSize
CQGradientPaletteDefinedColorsDelegate::
sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTableWidgetItem *item = colors_->item(index.row(), index.column());
  assert(item);

  return QItemDelegate::sizeHint(option, index);
}

void
CQGradientPaletteDefinedColorsDelegate::
updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                     const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

void
CQGradientPaletteDefinedColorsDelegate::
paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QTableWidgetItem *item = colors_->item(index.row(), index.column());
  assert(item);

  QItemDelegate::paint(painter, option, index);
}
