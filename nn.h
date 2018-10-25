#ifndef ZNN_H
#define ZNN_H

#include <vector>
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

    void SetN(int n) { zN = n; }
    void SetWeight(int n, double val) { zvWeight[n - 1] = val; }
    void SetBias(int n, double val) { zvBias[n - 1] = val; }
    void SetCutY(double val) { zCutY = val; }

    int GetN() { return zN; }
    double GetWeight(int n) { return zvWeight[n - 1]; }
    double GetBias(int n) { return zvBias[n - 1]; }
    double GetCutY() { return zCutY; }

    double Compute(const std::vector<double>& x);
    double Compute(std::vector<ZData>* vData);
    double Success(const std::vector<ZData>* vData, const int flagSample);

    std::vector<std::pair<double, double> > GetBorder(const int n, const double minX, const double maxX, const double minY, const double maxY);
    TF2* GetFunction(const std::vector<ZData>* vData);
};

#endif // ZNN_H
