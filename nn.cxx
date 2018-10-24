#include "nn.h"
#include <cmath>
#include <cstdio>

ZNN::ZNN()
{
  zFlagDebug = 0;
  zvWeight = std::vector<double>(6, 1.0);
  zvBias = std::vector<double>(3, 1.0);
}

double ZNN::activation(const double x)
{
  double y = tanh(x);
  return y;
}

double ZNN::Compute(const double x1, const double x2)
{
  double y11 = zvWeight[0] * x1 + zvWeight[1] * x2;
  double y12 = zvWeight[2] * x1 + zvWeight[3] * x2;

  y11 += zvBias[0];
  y12 += zvBias[1];

  y11 = activation(y11);
  y12 = activation(y12);

  double y22 = zvWeight[4] * y11 + zvWeight[5] * y12;
  y22 += zvBias[2];
  y22 = activation(y22);

  if(zFlagDebug)
    printf("x1 = %f  x2 = %f    y = %f\n", x1, x2, y22);

  return y22;
}
