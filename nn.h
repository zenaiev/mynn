#ifndef ZNN_H
#define ZNN_H

#include <vector>

class ZNN
{
  private:
    int zFlagDebug = 0;
    std::vector<double> zvWeight;
    std::vector<double> zvBias;

    double activation(const double x);

  public:
    ZNN();
    double Compute(const double x1, const double x2);
};

#endif // ZNN_H
