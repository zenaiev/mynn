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
  assert(npar == gNN->GetNPars());
  for(int i = 0; i < npar; i++)
    gNN->SetParameter(i, par[i]);

  //calculate chisquare
  gNN->Compute(gVData);
  Double_t success = gNN->Success(gVData, 0);
  f = 1.0 - success;
  nCallFCN++;
  if(nCallFCN % 100 == 0)
    printf("fcn[%d]: %f\n", nCallFCN, f);
}

#endif // FCN_H
