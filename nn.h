#ifndef ZNN_H
#define ZNN_H

#include <vector>
class ZData;
class TF2;

class ZNN
{
  private:
    int zFlagDebug = 0;
    std::vector<double> zvWeight;
    std::vector<double> zvBias;
    double zCutY;

    double activation(const double x);

  public:
    ZNN();

    void SetWeight(int n, double val) { zvWeight[n - 1] = val; }
    void SetBias(int n, double val) { zvBias[n - 1] = val; }
    void SetCutY(double val) { zCutY = val; }

    double GetWeight(int n) { return zvWeight[n - 1]; }
    double GetBias(int n) { return zvBias[n - 1]; }
    double GetCutY() { return zCutY; }

    double Compute(const double x1, const double x2);
    double Compute(std::vector<ZData>* vData);
    double Success(const std::vector<ZData>* vData, const int flagSample);

    std::vector<std::pair<double, double> > GetBorder(const int n, const double minX, const double maxX, const double minY, const double maxY);
    TF2* GetFunction(const std::vector<ZData>* vData);
};

#endif // ZNN_H
