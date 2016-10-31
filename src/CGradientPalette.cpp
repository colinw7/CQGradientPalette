#include <CGradientPalette.h>
#include <CUnixFile.h>
#include <CStrUtil.h>
#include <CAngle.h>
#include <NaN.h>

namespace Util {
  inline double clamp(double val, double low, double high) {
    if (val < low ) return low;
    if (val > high) return high;
    return val;
  }

  CExprTokenStack compileExpression(CExpr *expr, const std::string &str) {
    CExprTokenStack pstack = expr->parseLine(str);
    CExprITokenPtr  itoken = expr->interpPTokenStack(pstack);

    CExprTokenStack cstack;

    if (itoken.isValid())
      cstack = expr->compileIToken(itoken);
    else
      expr->errorMsg("Eval failed: '" + str + "'");

    return cstack;
  }
}

//------

CGradientPalette::
CGradientPalette(CExpr *expr) :
 expr_(expr)
{
  if (expr_)
    expr_->createRealVariable("pi", M_PI);

  setRedFunction  ("gray");
  setGreenFunction("gray");
  setBlueFunction ("gray");
}

void
CGradientPalette::
setExpr(CExpr *expr)
{
  expr_ = expr;
}

void
CGradientPalette::
setRedFunction(const std::string &fn)
{
  rf_.fn = fn;

  if (expr_)
    rf_.stack = Util::compileExpression(expr_, fn);
}

void
CGradientPalette::
setGreenFunction(const std::string &fn)
{
  gf_.fn = fn;

  if (expr_)
    gf_.stack = Util::compileExpression(expr_, fn);
}

void
CGradientPalette::
setBlueFunction(const std::string &fn)
{
  bf_.fn = fn;

  if (expr_)
    bf_.stack = Util::compileExpression(expr_, fn);
}

void
CGradientPalette::
setFunctions(const std::string &rf, const std::string &gf, const std::string &bf)
{
  colorType_ = ColorType::FUNCTIONS;

  setRedFunction  (rf);
  setGreenFunction(gf);
  setBlueFunction (bf);
}

void
CGradientPalette::
setCubeHelix(double start, double cycles, double saturation)
{
  colorType_ = ColorType::CUBEHELIX;

  setCbStart     (start);
  setCbCycles    (cycles);
  setCbSaturation(saturation);
}

void
CGradientPalette::
setRgbModel(int r, int g, int b)
{
  colorType_ = ColorType::MODEL;

  rModel_ = r;
  gModel_ = g;
  bModel_ = b;
}

void
CGradientPalette::
addDefinedColor(double v, const CColor &c)
{
  colors_[v] = c;
}

void
CGradientPalette::
resetDefinedColors()
{
  colors_.clear();
}

CColor
CGradientPalette::
getColor(double x) const
{
  if      (colorType() == ColorType::DEFINED) {
    if (colors().empty()) {
      CRGBA c1(0,0,0);
      CRGBA c2(1,1,1);

      return CColor::interp(c1, c2, x);
    }

    double min = colors(). begin()->first;
    double max = colors().rbegin()->first;

    auto p = colors().begin();

    double x1 = ((*p).first - min)/(max - min);
    CColor c1 = (*p).second;

    if (x <= x1) return c1;

    for (++p; p != colors().end(); ++p) {
      double x2 = ((*p).first - min)/(max - min);
      CColor c2 = (*p).second;

      if (x <= x2) {
        double m = (x - x1)/(x2 - x1);

        return CColor::interp(c1, c2, m);
      }

      x1 = x2;
      c1 = c2;
    }

    return c1;
  }
  else if (colorType() == ColorType::MODEL) {
    if (isNegative())
      x = 1.0 - x;

    if (isGray()) {
      double g = Util::clamp(x, 0.0, 1.0);

      return CColor(CRGBA(g, g, g));
    }
    else {
      double r = Util::clamp(interp(redModel  (), x), 0.0, 1.0);
      double g = Util::clamp(interp(greenModel(), x), 0.0, 1.0);
      double b = Util::clamp(interp(blueModel (), x), 0.0, 1.0);

      return CColor(CRGBA(r, g, b));
    }
  }
  else if (colorType() == ColorType::FUNCTIONS) {
    if (! expr_)
      return CColor(CRGBA(0,0,0));

    (void) expr_->createRealVariable("gray", x);

    bool oldQuiet = expr_->getQuiet();

    expr_->setQuiet(true);

    double r, g, b;

    CExprValuePtr value;

    if (! expr_->executeCTokenStack(rf_.stack, value) ||
        ! value.isValid() || ! value->getRealValue(r))
      r = 0.0;

    if (! expr_->executeCTokenStack(gf_.stack, value) ||
        ! value.isValid() || ! value->getRealValue(g))
      g = 0.0;

    if (! expr_->executeCTokenStack(bf_.stack, value) ||
        ! value.isValid() || ! value->getRealValue(b))
      b = 0.0;

    r = Util::clamp(r, 0, 1);
    g = Util::clamp(g, 0, 1);
    b = Util::clamp(b, 0, 1);

    CColor c;

    if      (colorModel() == ColorModel::RGB)
      c = CColor(CRGBA(r, g, b));
    else if (colorModel() == ColorModel::HSV)
      c = CColor(CHSV (360*r, g, b));
    else if (colorModel() == ColorModel::CMY)
      c = CColor(CHSV (r, g, b)); // TODO
    else if (colorModel() == ColorModel::YIQ)
      c = CColor(CHSV (r, g, b)); // TODO
    else if (colorModel() == ColorModel::XYZ)
      c = CColor(CHSV (r, g, b)); // TODO
    else
      c = CColor(CRGBA(r, g, b));

    expr_->setQuiet(oldQuiet);

    return c;
  }
  else if (colorType() == ColorType::CUBEHELIX) {
    return CColor(cubeHelix_.interp(x));
  }
  else {
    return CColor(CRGBA(0, 0, 0));
  }
}

double
CGradientPalette::
interp(int ind, double x) const
{
  if (ind < 0)
    return interp(-ind, 1 - x);

  switch (ind) {
    case  0: return 0;
    case  1: return 0.5;
    case  2: return 1;
    case  3: return x;
    case  4: return pow(x, 2);
    case  5: return pow(x, 3);
    case  6: return pow(x, 4);
    case  7: return sqrt(x);
    case  8: return sqrt(sqrt(x));
    case  9: return sin(CAngle::Deg2Rad(90*x));
    case 10: return cos(CAngle::Deg2Rad(90*x));
    case 11: return fabs(x - 0.5);
    case 12: return pow(2*x - 1, 2);
    case 13: return sin(CAngle::Deg2Rad(180*x));
    case 14: return fabs(cos(CAngle::Deg2Rad(180*x)));
    case 15: return sin(CAngle::Deg2Rad(360*x));
    case 16: return cos(CAngle::Deg2Rad(360*x));
    case 17: return fabs(sin(CAngle::Deg2Rad(360*x)));
    case 18: return fabs(cos(CAngle::Deg2Rad(360*x)));
    case 19: return fabs(sin(CAngle::Deg2Rad(720*x)));
    case 20: return fabs(cos(CAngle::Deg2Rad(720*x)));
    case 21: return 3*x;
    case 22: return 3*x - 1;
    case 23: return 3*x - 2;
    case 24: return fabs(3*x - 1);
    case 25: return fabs(3*x - 2);
    case 26: return (3*x - 1)/2;
    case 27: return (3*x - 2)/2;
    case 28: return fabs((3*x - 1)/2);
    case 29: return fabs((3*x - 2)/2);
    case 30: return x/0.32 - 0.78125;
    case 31: return 2*x - 0.84;
    case 32: {
      if      (x < 0.00) return 0;
      else if (x < 0.25) return 4*x;
      else if (x < 0.42) return 1;
      else if (x < 0.92) return -2*x + 1.84;
      else if (x < 1.00) return x/0.08 - 11.5;
      else               return 1;
    }
    case 33: return fabs(2*x - 0.5);
    case 34: return 2*x;
    case 35: return 2*x - 0.5;
    case 36: return 2*x - 1;
    default: return x;
  }
}

std::string
CGradientPalette::
modelName(int ind)
{
  if (ind < 0)
    return "Inverted " + modelName(-ind);

  switch (ind) {
    case  0: return "0";
    case  1: return "0.5";
    case  2: return "1";
    case  3: return "x";
    case  4: return "x\u00B2";
    case  5: return "x\u00B3";
    case  6: return "x\u2074";
    case  7: return "\u221Ax";
    case  8: return "\u221A\u221Ax";
    case  9: return "sin(90*x)";
    case 10: return "cos(90*x)";
    case 11: return "\u007Cx - 0.5\u007C";
    case 12: return "(2*x - 1)\u00B2";
    case 13: return "sin(180*x)";
    case 14: return "\u007Ccos(180*x)\u007C";
    case 15: return "sin(360*x)";
    case 16: return "cos(360*x)";
    case 17: return "\u007Csin(360*x)\u007C";
    case 18: return "\u007Ccos(360*x)\u007C";
    case 19: return "\u007Csin(720*x)\u007C";
    case 20: return "\u007Ccos(720*x)\u007C";
    case 21: return "3*x";
    case 22: return "3*x - 1";
    case 23: return "3*x - 2";
    case 24: return "\u007C3*x - 1\u007C";
    case 25: return "\u007C3*x - 2\u007C";
    case 26: return "(3*x - 1)/2";
    case 27: return "(3*x - 2)/2";
    case 28: return "\u007C(3*x - 1)/2\u007C";
    case 29: return "\u007C(3*x - 2)/2\u007C";
    case 30: return "x/0.32 - 0.78125";
    case 31: return "2*x - 0.84";
    case 32: return "0,4*x,1,-2*x + 1.84,x/0.08 - 11.5,1";
    case 33: return "\u007C2*x - 0.5\u007C";
    case 34: return "2*x";
    case 35: return "2*x - 0.5";
    case 36: return "2*x - 1";
    default: return "x";
  }
}

bool
CGradientPalette::
readFile(const std::string &filename)
{
  typedef std::vector<std::string> Lines;

  Lines lines;

  CUnixFile file(filename);

  if (! file.open()) {
    std::cerr << "can't open file \"" << filename << "\"" << std::endl;
    return false;
  }

  std::string line;

  while (file.readLine(line)) {
    lines.push_back(line);
  }

  if (! readFileLines(lines))
    return false;

  return true;
}

bool
CGradientPalette::
readFileLines(const std::vector<std::string> &lines)
{
  setColorType(CGradientPalette::ColorType::DEFINED);

  resetDefinedColors();

  int i = 0;

  for (const auto &line : lines) {
    if (line.empty()) continue;

    std::vector<std::string> words;

    CStrUtil::addWords(line, words);

    double x = i;

    int j = 0;

    if (words.size() >= 4) {
      x = CStrUtil::toReal(words[0]);

      ++j;
    }

    double r, g, b;

    if (words.size() >= 3) {
      r = CStrUtil::toReal(words[j + 0]);
      g = CStrUtil::toReal(words[j + 1]);
      b = CStrUtil::toReal(words[j + 2]);
    }
    else
      continue;

    addDefinedColor(x, CRGBA(r, g, b));

    ++i;
  }

  return true;
}

void
CGradientPalette::
unset()
{
  colorType_ = ColorType::MODEL;

  // Model
  rModel_ = 7;
  gModel_ = 5;
  bModel_ = 15;

  // Defined
  colors_.clear();

  // Functions
  setRedFunction  ("gray");
  setGreenFunction("gray");
  setBlueFunction ("gray");

  // CubeHelix
  cubeHelix_.reset();

  // Misc
  colorModel_ = ColorModel::RGB;
  negative_   = false;
  gray_       = false;
  gamma_      = 1.5;
  maxColors_  = -1;
  psAllcF_    = false;
}

void
CGradientPalette::
show(std::ostream &os) const
{
  os << "palette is " << (isGray() ? "GRAY" : "COLOR") << std::endl;

  if (! isGray()) {
    if      (colorType() == ColorType::MODEL)
      os << "rgb color mapping by rgbformulae are " <<
            redModel() << "," << greenModel() << "," << blueModel() << std::endl;
    else if (colorType() == ColorType::DEFINED)
      os << "color mapping by defined gradient" << std::endl;
    else if (colorType() == ColorType::FUNCTIONS)
      os << "color mapping is done by user defined functions " <<
            "A-formula: " << redFunction() <<
            "B-formula: " << greenFunction() <<
            "C-formula: " << blueFunction() << std::endl;
    else if (colorType() == ColorType::CUBEHELIX)
      os << "Cubehelix color palette: start " << cubeHelix_.start() <<
            " cycles " << cubeHelix_.cycles() <<
            " saturation " << cubeHelix_.saturation() << std::endl;
  }

  os << "figure is " << (isNegative() ? "NEGATIVE" : "POSITIVE") << std::endl;

  if (psAllcF_)
    os << "all color formulae ARE written into output postscript file" << std::endl;
  else
    os << "all color formulae ARE NOT written into output postscript file" << std::endl;

  if (maxColors() <= 0)
    os << "allocating ALL remaining";
  else
    os << "allocating MAX " << maxColors();
  os << " color positions for discrete palette terminals" << std::endl;

  os << "Color-Model: ";
  if      (colorModel() == ColorModel::RGB) os << "RGB";
  else if (colorModel() == ColorModel::HSV) os << "HSV";
  else if (colorModel() == ColorModel::CMY) os << "CMY";
  else if (colorModel() == ColorModel::YIQ) os << "YIQ";
  else if (colorModel() == ColorModel::XYZ) os << "XYZ";
  os << std::endl;

  os << "gamma is " << gamma_ << std::endl;
}

void
CGradientPalette::
showGradient(std::ostream &os) const
{
  if (colorType() != ColorType::DEFINED) {
    int i = 0;

    for (const auto &cc : colors()) {
      const CColor &c = cc.second;

      os << i << ". gray=" << cc.first << ", ";

      if (c.type() == CColor::Type::RGB) {
        CRGBA rgba = c.rgba();

        os << "(r,g,b)=(" << rgba.getRed() << "," << rgba.getGreen() << "," << rgba.getBlue() <<
              "), " << rgba.getRGB().stringEncode() << " = " <<
              rgba.getRedI() << " " << rgba.getGreenI() << " " << rgba.getBlueI() << std::endl;
      }
      else if (c.type() == CColor::Type::HSV) {
        // TODO
        os << "(h,s,v)=(...)";
      }

      ++i;
    }
  }
  else
    os << "color mapping *not* done by defined gradient." << std::endl;
}

void
CGradientPalette::
showRGBFormulae(std::ostream &os) const
{
  os << "set palette rgbformulae " <<
        redModel() << "," << greenModel() << "," << blueModel() << std::endl;
}

void
CGradientPalette::
showPaletteValues(std::ostream &os, int n, bool is_float, bool is_int)
{
  if (n <= 1) n = 128;

  os << "Color palette with " << n << " discrete colors." << std::endl;

  double x = 0.0;
  double d = 1.0/(n - 1);

  for (int i = 0; i < n; ++i, x += d) {
    CColor c = getColor(x);

    os << "  ";

    if      (c.type() == CColor::Type::RGB) {
      CRGBA rgba = c.rgba();

      if      (is_float) {
        os << rgba.getRed() << " " << rgba.getGreen() << " " << rgba.getBlue() << std::endl;
      }
      else if (is_int) {
        os << rgba.getRedI() << " " << rgba.getGreenI() << " " << rgba.getBlueI() << std::endl;
      }
      else {
        os << i << ". gray=" << x << ", (r,g,b)=(" <<
              rgba.getRed() << "," << rgba.getGreen() << "," << rgba.getBlue() <<
              "), " << rgba.getRGB().stringEncode() << " = " <<
              rgba.getRedI() << " " << rgba.getGreenI() << " " << rgba.getBlueI() << std::endl;
      }
    }
    else if (c.type() == CColor::Type::HSV) {
      // TODO
    }
  }
}
