#include "nn.h"
#include "data.h"
#include <cstdio>

int main(int argc, char** argv)
{
  ZNN* nn = new ZNN();

  double x1 = 2.5;
  double x2 = -1.5;
  //double y = nn->Compute(x1, x2);
  //printf("x1 = %f  x2 = %f    y = %f\n", x1, x2, y);

  //nn->Compute(0.1, 0.3);
  //nn->Compute(10.0, 5.0);
  //nn->Compute(-0.5, -0.9);
  //nn->Compute(0.01, -0.002);

  // generate data
  const int dataN = 10000;
  std::vector<ZData>* vData = ZData::GenerateData(dataN);
  ZData::DrawData(vData, "plots/data");
  //std::vector<std::> dataX(dataN);

  // calculate NN
  for(size_t i = 0; i < vData->size(); i++)
  {
    ZData& data = (*vData)[i];
    data.Y = nn->Compute(data.X1, data.X2);
  }
  ZData::DrawNNY(vData, "plots/nny");


  return 0;
}
