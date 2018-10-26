#include "nn.h"
#include "data.h"
#include "fcn.h"
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <TMinuit.h>

int gNVar = 2;

std::vector<std::string> split(const std::string& strIn, const std::string& delim)
{
  // omit "name"
  size_t pos1 = strIn.find('"', 0);
  size_t pos2 = strIn.find('"', pos1 + 1);
  std::string str = std::string(strIn.c_str(), pos1) + std::string(strIn.c_str() + pos2 + 1);
  // remove trailing chars
  //std::remove(str.begin(), str.end(), '\r');
  str = std::string(str.c_str(), str.size() - 2);

  std::vector<std::string> tokens;
  size_t prev = 0, pos = 0;
  do
  {
    pos = str.find(delim, prev);
    if(pos == std::string::npos)
      pos = str.length();
    std::string token = str.substr(prev, pos - prev);
    //if (!token.empty())
    tokens.push_back(token);
    prev = pos + delim.length();
  }
  while (pos <= str.length() && prev <= str.length());
  return tokens;
}

std::vector<ZData>* ReadDataTitanic(const std::string& fileName, const bool flagTrain)
{
  std::vector<ZData>* vData = new std::vector<ZData>(1000);
  FILE* f = fopen(fileName.c_str(), "r");
  char buff[2048];
  // skip 1st line
  fgets(buff, 2047, f);
  int count = 0;
  while(true)
  {
    fgets(buff, 2047, f);
    if(feof(f))
      break;
    const std::vector<std::string>& tokens = split(buff, ",");
    assert(tokens.size() == 12 && flagTrain || tokens.size() == 11 && !flagTrain);
    int column = 0;
    count++;
    if(count >= vData->size())
      vData->resize(count);
    ZData& data = vData->at(count - 1);
    data.X.resize(gNVar);

    // check id
    int id = atoi(tokens[column].c_str());
    assert(count == id || !flagTrain);

    // survived
    bool flagSurvived = false;
    if(flagTrain)
    {
      column++;
      flagSurvived = atoi(tokens[column].c_str());
      data.Origin = flagSurvived;
      data.Sample = 0;
    }
    else
      data.Sample = 1;
    int xUsed = 0;

    // pclass
    column++;
    int pclass = atoi(tokens[column].c_str());
    assert(pclass == 1 || pclass == 2 || pclass == 3);
    //data.X[xUsed++] = pclass;

    // name: do not use
    column++;

    // sex
    column++;
    int sex = -1;
    if(tokens[column] == "male")
      sex = 2;
    else if(tokens[column] == "female")
      sex = 1;
    else
      assert(0);
    data.X[xUsed++] = sex;

    // age
    column++;
    int age = -1;
    if(tokens[column] != "")
      age = atoi(tokens[column].c_str());
    //data.X[xUsed++] = age;

    // SibSp
    column++;
    int sibsp = atoi(tokens[column].c_str());
    //data.X[xUsed++] = sibsp;

    // parch
    column++;
    int parch = atoi(tokens[column].c_str());
    //data.X[xUsed++] = parch;

    // ticket: do not use
    column++;

    // fare
    column++;
    int fare = atof(tokens[column].c_str());
    //data.X[xUsed++] = fare;

    // cabin: do not use
    column++;

    // embarked
    column++;
    int embarked = -1;
    if(tokens[column] == 'C')
      embarked = 1;
    else if(tokens[column] == 'Q')
      embarked = 2;
    else if(tokens[column] == 'S')
      embarked = 1;
    else if(tokens[column].size() == 0)
      embarked = -1;
    else
      assert(0);
    data.X[xUsed++] = embarked;

    assert(column == 11 && flagTrain || column == 10 && !flagTrain);
  }
  vData->resize(count);
  return vData;
}

int main(int argc, char** argv)
{
  ZNN* nn = new ZNN();
  nn->SetN(gNVar);
  for(int i = 0; i < gNVar; i++)
  {
    //for(int j = 0; j < gNVar; j++)
    //  nn->SetWeight(i * gNVar + j + 1, (i * gNVar + j + 1) / 100.0);
    //nn->SetWeight(gNVar * gNVar + i + 1, (gNVar * gNVar + i + 1) / 100.0);
    //nn->SetBias(i + 1, (i + 1) / 10000.0);
    for(int j = 0; j < gNVar; j++)
      nn->SetWeight(i * gNVar + j + 1, (i * gNVar + j + 1) / 30.0);
    nn->SetWeight(gNVar * gNVar + i + 1, (gNVar * gNVar + i + 1) / 30.0);
    nn->SetBias(i + 1, (i + 1) / 5.0);
    //const int seed = 42;
    //std::default_random_engine generator(seed);
    //std::uniform_real_distribution<double> distrUniform(-2.0, 2.0);
    //for(int j = 0; j < gNVar; j++)
    //  nn->SetWeight(i * gNVar + j + 1, distrUniform(generator));
    //nn->SetWeight(gNVar * gNVar + i + 1, distrUniform(generator));
    //nn->SetBias(i + 1, distrUniform(generator));
    //for(int j = 0; j < gNVar; j++)
    //  nn->SetWeight(i * gNVar + j + 1, 0.5);
    //nn->SetWeight(gNVar * gNVar + i + 1, 0.5);
    //nn->SetBias(i + 1, 0.5);
  }
  nn->SetBias(gNVar + 1, 0.09);
  //nn->SetBias(gNVar + 1, 0.0);
  nn->SetCutY(0.01);
  nn->PrintPars();
  std::vector<ZData>* vDataTrain = ReadDataTitanic("titanic-data/train.csv", 1);
  std::vector<ZData>* vDataTest = ReadDataTitanic("titanic-data/test.csv", 0);

  nn->Compute(vDataTrain);
  printf("success: %f\n", nn->Success(vDataTrain, 0));
  //return 0;

  // minimise fcn
  gNN = nn;
  gVData = vDataTrain;
  TMinuit *gMinuit = new TMinuit(gNVar * gNVar + gNVar + gNVar + 1 + 1);
  gMinuit->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg = 0;

  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist, 1, ierflg);

  // Set starting values and step sizes for parameters
  for(int i = 0; i < nn->GetNPars(); i++)
  {
    char name[32];
    if(i < gNVar * gNVar + gNVar)
      sprintf(name, "w%d", i);
    else if(i < gNVar * gNVar + gNVar + gNVar + 1)
      sprintf(name, "b%d", i);
    else if(i < nn->GetNPars())
      sprintf(name, "cut");
    gMinuit->mnparm(i, name, nn->GetParameter(i), fabs(nn->GetParameter(i) / 10) + 0.5, 0, 0, ierflg);
    //gMinuit->mnparm(i, name, nn->GetParameter(i), fabs(nn->GetParameter(i) / 2) + 0.9, 0, 0, ierflg);
    //if(i >= nvar * nvar + nvar && i < nvar * nvar + nvar + nvar + 1)
    //  gMinuit->FixParameter(i);
  }

  // Now ready for minimization step
  for(int ii = 0; ii < 5; ii++)
  {
    for(int i = 0; i < nn->GetNPars(); i++)
    {
      arglist[0] = i;
      arglist[1] = 80;
      gMinuit->mnexcm("SCAN", arglist, 2, ierflg);
    }
    arglist[0] = 500000;
    arglist[1] = 1.;
    //gMinuit->mnexcm("SEEK", arglist, 2, ierflg);
    gMinuit->mnexcm("MINIMIZE", arglist, 2, ierflg);
  }
  arglist[0] = 500000;
  arglist[1] = 1.;
  //gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
  gMinuit->mnexcm("HESSE", arglist, 2, ierflg);

  // Print results
  Double_t amin, edm, errdef;
  Int_t nvpar, nparx, icstat;
  gMinuit->mnstat(amin, edm, errdef, nvpar, nparx, icstat);
  //gMinuit->mnprin(3,amin);

  double val = -1.0;
  double err = -1.0;
  for(int i = 0; i < nn->GetNPars(); i++)
  {
    gMinuit->GetParameter(i, val, err);
    gNN->SetParameter(i, val);
  }
  nn->Compute(vDataTrain);
  //ZData::DrawNNY(vData, "plots/nny");
  printf("success: %f\n", nn->Success(vDataTrain, 0));
  //printf("success[train]: %f\n", nn->Success(vData, 0));
  //printf("success[valid]: %f\n", nn->Success(vData, 1));
  //border = nn->GetBorder(100, -6.0, 6.0, -6.0, 6.0);
  //f = nn->GetFunction(vData);
  //ZData::DrawData(vData, "plots/data-final", true, NULL, f);*/

  int successWomen = 0;
  for(size_t i = 0; i < vDataTrain->size(); i++)
  {
    ZData& data = (*vDataTrain)[i];
    int sex = data.X[1];
    if(data.Origin == 1 && sex == 1 || data.Origin == 0 && sex == 2)
      successWomen++;
  }
  printf("successWomen: %f\n", 1.0 * successWomen / vDataTrain->size());

  // output
  nn->Compute(vDataTest);
  FILE* fout = fopen("titanic-data/submission-251018.csv", "w");
  fprintf(fout, "PassengerId,Survived\r\n");
  for(size_t i = 0; i < vDataTest->size(); i++)
  {
    ZData& data = (*vDataTest)[i];
    fprintf(fout, "%lu,%d\r\n", i + 892, data.Prediction);
  }
  fclose(fout);

  FILE* fout1 = fopen("titanic-data/submissionGender-251018.csv", "w");
  fprintf(fout1, "PassengerId,Survived\r\n");
  for(size_t i = 0; i < vDataTest->size(); i++)
  {
    ZData& data = (*vDataTest)[i];
    int sex = data.X[1];
    fprintf(fout1, "%lu,%d\r\n", i + 892, (sex == 1));
  }
  fclose(fout1);

  return 0;
}
