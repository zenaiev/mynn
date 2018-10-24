#include "nn.h"
#include "data.h"
#include "fcn.h"
#include <cstdio>
#include <TMinuit.h>

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
  const int dataN = 25000;
  std::vector<ZData>* vData = ZData::GenerateData(dataN);
  ZData::DrawData(vData, "plots/data");
  //std::vector<std::> dataX(dataN);

  // calculate NN
  //for(size_t i = 0; i < vData->size(); i++)
  //{
  //  ZData& data = (*vData)[i];
  //  data.Y = nn->Compute(data.X1, data.X2);
  //}
  nn->Compute(vData);
  ZData::DrawNNY(vData, "plots/nny");

  // calculate success
  double success = nn->Success(vData, -1);
  printf("success: %f\n", success);
  ZData::DrawData(vData, "plots/data-start", true);

  // minimise fcn
  gNN = nn;
  gVData = vData;
  TMinuit *gMinuit = new TMinuit(10);
  gMinuit->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg = 0;

  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist, 1, ierflg);

  // Set starting values and step sizes for parameters
  static Double_t vstart[10] = { nn->GetWeight(1), nn->GetWeight(2), nn->GetWeight(3), nn->GetWeight(4),
                                nn->GetWeight(5), nn->GetWeight(6),
                                 nn->GetBias(1), nn->GetBias(2), nn->GetBias(3), nn->GetCutY() };
  static Double_t step[10] = { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };
  gMinuit->mnparm(0, "w1", vstart[0], step[0], 0, 0, ierflg);
  gMinuit->mnparm(1, "w2", vstart[1], step[1], 0, 0, ierflg);
  gMinuit->mnparm(2, "w3", vstart[2], step[2], 0, 0, ierflg);
  gMinuit->mnparm(3, "w4", vstart[3], step[3], 0, 0, ierflg);
  gMinuit->mnparm(4, "w5", vstart[4], step[4], 0, 0, ierflg);
  gMinuit->mnparm(5, "w6", vstart[5], step[5], 0, 0, ierflg);
  gMinuit->mnparm(6, "b1", vstart[6], step[6], 0, 0, ierflg);
  gMinuit->mnparm(7, "b2", vstart[7], step[7], 0, 0, ierflg);
  gMinuit->mnparm(8, "b3", vstart[8], step[8], 0, 0, ierflg);
  gMinuit->mnparm(9, "cut", vstart[9], step[9], -1.0, 1.0, ierflg);

  // Now ready for minimization step
  arglist[0] = 5000;
  arglist[1] = 1.;
  gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
  gMinuit->mnexcm("HESSE", arglist, 2, ierflg);

  // Print results
  Double_t amin, edm, errdef;
  Int_t nvpar, nparx, icstat;
  gMinuit->mnstat(amin, edm, errdef, nvpar, nparx, icstat);
  //gMinuit->mnprin(3,amin);

  double val = -1.0;
  double err = -1.0;
  for(int i = 0; i < 6; i++)
  {
    gMinuit->GetParameter(i, val, err);
    gNN->SetWeight(i + 1, val);
  }
  for(int i = 6; i < 9; i++)
  {
    gMinuit->GetParameter(i, val, err);
    gNN->SetBias(i - 5, val);
  }
  gMinuit->GetParameter(9, val, err);
  gNN->SetCutY(val);
  nn->Compute(vData);
  ZData::DrawNNY(vData, "plots/nny");
  printf("success: %f\n", nn->Success(vData, -1));
  printf("success[train]: %f\n", nn->Success(vData, 0));
  printf("success[valid]: %f\n", nn->Success(vData, 1));
  ZData::DrawData(vData, "plots/data-final", true);

  return 0;
}
