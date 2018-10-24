#ifndef FCN_H
#define FCN_H

#include "nn.h"
#include <cassert>
#include <TString.h>

std::vector<ZData>* gVData = NULL;
ZNN* gNN = NULL;
int nCallFCN = 0;
//______________________________________________________________________________
void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  assert(npar == 10);
  gNN->SetWeight(1, par[0]);
  gNN->SetWeight(2, par[1]);
  gNN->SetWeight(3, par[2]);
  gNN->SetWeight(4, par[3]);
  gNN->SetWeight(5, par[4]);
  gNN->SetWeight(6, par[5]);
  gNN->SetBias(1, par[6]);
  gNN->SetBias(2, par[7]);
  gNN->SetBias(3, par[8]);
  gNN->SetCutY(par[9]);

  //calculate chisquare
  gNN->Compute(gVData);
  Double_t success = gNN->Success(gVData, 0);
  f = 1.0 - success;
  nCallFCN++;
  //printf("fcn[%d]: %f\n", nCallFCN, f);
}

#endif // FCN_H
