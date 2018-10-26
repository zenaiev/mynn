#ifndef ZNN_H
#define ZNN_H

#include <vector>
#include <cassert>
class ZData;
class TF2;

class ZNN
{
  private:
    int zFlagDebug = 0;
    int zN;
    std::vector<double> zvWeight;
    std::vector<double> zvBias;
    double zCutY;

    double activation(const double x);

  public:
    ZNN();

    void SetN(int n) { zN = n; zvWeight.resize(zN * zN + zN); zvBias.resize(zN + 1); }
    void SetWeight(int n, double val) { zvWeight[n - 1] = val; }
    void SetBias(int n, double val) { zvBias[n - 1] = val; }
    void SetCutY(double val) { zCutY = val; }
    void SetParameter(int n, double val)
    {
      if(n < zN * zN + zN)
        SetWeight(n + 1, val);
      else if(n < zN * zN + zN + zN + 1)
        SetBias(n + 1 - zN * zN - zN, val);
      else if(n < zN * zN + zN + zN + 1 + 1)
        SetCutY(val);
      else
        assert(0);
    }

    int GetN() { return zN; }
    double GetWeight(int n) { return zvWeight[n - 1]; }
    double GetBias(int n) { return zvBias[n - 1]; }
    double GetCutY() { return zCutY; }
    double GetParameter(int n)
    {
      if(n < zN * zN + zN)
        return GetWeight(n + 1);
      else if(n < zN * zN + zN + zN + 1)
        return GetBias(n + 1 - zN * zN - zN);
      else if(n < zN * zN + zN + zN + 1 + 1)
        return GetCutY();
      else
      {
        assert(0);
        return 0.0;
      }
    }
    int GetNPars() { return zN * zN + zN + zN + 1 + 1; }

    double Compute(const std::vector<double>& x);
    double Compute(std::vector<ZData>* vData);
    double Success(const std::vector<ZData>* vData, const int flagSample);
    void PrintPars();

    std::vector<std::pair<double, double> > GetBorder(const int n, const double minX, const double maxX, const double minY, const double maxY);
    TF2* GetFunction(const std::vector<ZData>* vData);
};

#endif // ZNN_H
