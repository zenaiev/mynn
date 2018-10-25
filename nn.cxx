#include "nn.h"
#include "data.h"
#include <cmath>
#include <cstdio>
#include <cassert>
#include <TF2.h>

ZNN::ZNN()
{
  zFlagDebug = 0;
  zvWeight = std::vector<double>(6, -1.5);
  zvBias = std::vector<double>(3, 2.9);
  zvWeight[0] = 0.9;
  zvWeight[1] = 0.7;
  zvWeight[2] = 1.5;
  zvWeight[3] = 0.6;
  zvWeight[4] = -0.4;
  zvWeight[5] = 0.9;
  zvBias[0] = -0.1;
  zvBias[1] = 0.2;
  zvBias[2] = -0.4;
  zCutY = 0.1;
}

double ZNN::activation(const double x)
{
  double y = tanh(x);
  return y;
}

double ZNN::Compute(const std::vector<double>& x)
{
  assert(x.size() == zN);
  std::vector<double> y(zN);
  for(int i = 0; i < zN; i++)
  {
    for(int j = 0; j < zN; j++)
      y[i] += zvWeight[i * zN + j] * x[j];
    y[i] += zvBias[i];
    y[i] = activation(y[i]);
  }

  double y2 = 0.0;
  for(int i = 0; i < zN; i++)
  {
    y2 += zvWeight[zN * zN + i] * y[i];
  }
  y2 += zvBias[zN];
  y2 = activation(y2);

  //if(zFlagDebug)
  //  printf("x1 = %f  x2 = %f    y = %f\n", x[0], x[1], y2);

  return y2;
}

double ZNN::Compute(std::vector<ZData>* vData)
{
  for(size_t i = 0; i < vData->size(); i++)
  {
    ZData& data = (*vData)[i];
    data.Y = Compute(data.X);
    if(data.Y < zCutY)
      data.Prediction = 0;
    else
      data.Prediction = 1;
  }
}

double ZNN::Success(const std::vector<ZData>* vData, const int flagSample)
{
  int nSuccess = 0;
  int nCheck = 0;
  for(size_t i = 0; i < vData->size(); i++)
  {
    const ZData& data = (*vData)[i];
    if(flagSample >= 0 && flagSample != data.Sample)
      continue;
    nCheck++;
    if(data.Y < zCutY && data.Origin == 0 || data.Y >= zCutY && data.Origin == 1)
      nSuccess++;
  }
  double success = 1.0 * nSuccess / nCheck;
  //printf("success: %f\n", success);
  return success;
}

std::vector<std::pair<double, double> > ZNN::GetBorder(const int n, const double minX, const double maxX, const double minY, const double maxY)
{
  std::vector<std::pair<double, double> > border(n + 1);
  for(int ix = 0; ix <= n; ix++)
  {
    double x = minX + (maxX - minX) * ix / n;
    double diffMin = 10.0;
    double yBest = 0.0;
    for(int iy = 0; iy <= n; iy++)
    {
      double y = minY + (maxY - minY) * iy / n;
      double diff = fabs(Compute({ x, y }) - zCutY);
      //printf("diff = %f\n", diff);
      if(diff < diffMin)
      {
        diffMin = diff;
        yBest = y;
      }
    }
    //if(yBest > 0.1)
    //  printf("Warning ZNN::GetBorder(): x = %f yBest = %f diffMin = %f\n", x, yBest, diffMin);
    border[ix] = std::pair<double, double>(x, yBest);
  }
  return border;
}

ZNN* gNNTF2(NULL);
double myFunc(double* x, double* par)
{
  double val = gNNTF2->Compute({ x[0], x[1] }) - gNNTF2->GetCutY();
  if(val > 0.0)
    val = 5.0;
  else
    val = -5.0;
  return val;
}

TF2* ZNN::GetFunction(const std::vector<ZData>* vData)
{
  assert(zN == 2);
  double minX = 0.0;
  double maxX = 0.0;
  double minY = 0.0;
  double maxY = 0.0;
  for(size_t i = 0; i < vData->size(); i++)
  {
    const ZData& data = (*vData)[i];
    if(data.X[0] < minX)
      minX = data.X[0];
    if(data.X[0] > maxX)
      maxX = data.X[0];
    if(data.X[1] < minY)
      minY = data.X[1];
    if(data.X[1] > maxY)
      maxY = data.X[1];
  }
  gNNTF2 = this;
  TF2* f = new TF2("", myFunc, minX, maxX, minY, maxY);
  return f;
}
