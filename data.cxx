#include "data.h"
#include <random>
#include <iostream>
#include <TGraph.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TF2.h>

ZData::ZData()
{
  X1 = 0.0;
  X2 = 0.0;
  Y = 0.0;
  Origin = -1;
  Prediction = -1;
  Sample = -1;
}

std::vector<ZData>* ZData::GenerateData(const int n, const double fractionSignal, const double fractionValid)
{
  const int seed = 42;
  std::default_random_engine generator(seed);
  std::normal_distribution<double> distrSigX1(MeanSigX1);
  std::normal_distribution<double> distrSigX2(MeanSigX2);
  std::normal_distribution<double> distrBkgX1(MeanBkgX1);
  std::normal_distribution<double> distrBkgX2(MeanBkgX2);
  std::uniform_real_distribution<double> distrUniform(0.0,1.0);

  std::vector<ZData>* vData = new std::vector<ZData>(n);
  for(int i = 0; i < n; i++)
  {
    ZData& data = (*vData)[i];
    if(fractionSignal * n < i)
    {
      // signal
      data.X1 = distrSigX1(generator);
      data.X2 = distrSigX2(generator);
      data.Origin = 1;
    }
    else
    {
      // background
      data.X1 = distrBkgX1(generator);
      data.X2 = distrBkgX2(generator);
      data.Origin = 0;
    }
    double valid = distrUniform(generator);
    if(fractionValid < valid)
      data.Sample = 1;
    else
      data.Sample = 0;
  }

  // draw data
  if(1 == 2)
  {
    const int nstars = 100;
    const double min = -3.0;
    const double max = 3.0;
    std::vector<int> pSigX1 = std::vector<int>(6, 0);
    std::vector<int> pSigX2 = std::vector<int>(6, 0);
    std::vector<int> pSigR = std::vector<int>(6, 0);
    std::vector<int> pBkgX1 = std::vector<int>(6, 0);
    std::vector<int> pBkgX2 = std::vector<int>(6, 0);
    std::vector<int> pBkgR = std::vector<int>(6, 0);
    for (int i = 0; i < n; i++)
    {
      const ZData& data = (*vData)[i];
      if(data.Origin == 1)
      {
        double number = data.X1;
        if((number >= min) && (number < max))
          ++pSigX1[int(number - min)];
        //printf("number = %f -> %d\n", number, int(number - min));

        number = data.X2;
        if((number >= min) && (number < max))
          ++pSigX2[int(number - min)];

        number = sqrt(data.X1 * data.X1 + data.X2 * data.X2);
        if((number >= min) && (number < max))
          ++pSigR[int(number - min)];
      }
      else
      {
        double number = data.X1;
        if((number >= min) && (number < max))
          ++pBkgX1[int(number - min)];

        number = data.X2;
        if((number >= min) && (number < max))
          ++pBkgX2[int(number - min)];

        number = sqrt(data.X1 * data.X1 + data.X2 * data.X2);
        if((number >= min) && (number < max))
          ++pBkgR[int(number - min)];
      }
    }

    std::cout << std::string(100, '-') << std::endl;
    std::cout << "data signal X1:" << std::endl;
    for(int i = min; i < max; i++)
    {
      std::cout << i << " to " << (i + 1) << ": ";
      std::cout << std::string(pSigX1[i - min] * nstars / n, '*') << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;

    std::cout << std::string(100, '-') << std::endl;
    std::cout << "data signal X2:" << std::endl;
    for(int i = min; i < max; i++)
    {
      std::cout << i << " to " << (i + 1) << ": ";
      std::cout << std::string(pSigX2[i - min] * nstars / n, '*') << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;

    std::cout << std::string(100, '-') << std::endl;
    std::cout << "data sig R:" << std::endl;
    for(int i = min; i < max; i++)
    {
      std::cout << i << " to " << (i + 1) << ": ";
      std::cout << std::string(pSigR[i - min] * nstars / n, '*') << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;

    std::cout << std::string(100, '-') << std::endl;
    std::cout << "data bkg X1:" << std::endl;
    for(int i = min; i < max; i++)
    {
      std::cout << i << " to " << (i + 1) << ": ";
      std::cout << std::string(pBkgX1[i - min] * nstars / n, '*') << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;

    std::cout << std::string(100, '-') << std::endl;
    std::cout << "data bkg X2:" << std::endl;
    for(int i = min; i < max; i++)
    {
      std::cout << i << " to " << (i + 1) << ": ";
      std::cout << std::string(pBkgX2[i - min] * nstars / n, '*') << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;

    std::cout << std::string(100, '-') << std::endl;
    std::cout << "data bkg R:" << std::endl;
    for(int i = min; i < max; i++)
    {
      std::cout << i << " to " << (i + 1) << ": ";
      std::cout << std::string(pBkgR[i - min] * nstars / n, '*') << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;
  }

  return vData;
}

void ZData::DrawData(const std::vector<ZData>* vData, const std::string& fileName, bool flagNN, const std::vector<std::pair<double, double> >* border, TF2* f)
{
  gStyle->SetOptStat(0000000000);
  gStyle->SetPalette(3);

  TGraph* grSig = new TGraph;
  grSig->SetMarkerColor(2);
  grSig->SetMarkerStyle(20);
  grSig->SetMarkerSize(0.35);

  TGraph* grBkg = new TGraph;
  grBkg->SetMarkerColor(4);
  grBkg->SetMarkerStyle(20);
  grBkg->SetMarkerSize(0.35);

  TGraph* grSigWrong = new TGraph;
  grSigWrong->SetMarkerColor(kRed - 6);
  grSigWrong->SetMarkerStyle(20);
  grSigWrong->SetMarkerSize(0.35);

  TGraph* grBkgWrong = new TGraph;
  grBkgWrong->SetMarkerColor(kBlue - 6);
  grBkgWrong->SetMarkerStyle(20);
  grBkgWrong->SetMarkerSize(0.35);

  TGraph* grBorder = NULL;
  if(border)
  {
    grBorder = new TGraph;
    for(size_t i = 0; i < border->size(); i++)
      grBorder->SetPoint(grBorder->GetN(), (*border)[i].first, (*border)[i].second);
  }

  double xMin = 0.0;
  double xMax = 0.0;
  double yMin = 0.0;
  double yMax = 0.0;
  for(size_t i = 0; i < vData->size(); i++)
  {
    const ZData& data = (*vData)[i];
    if(xMin > data.X1)
      xMin = data.X1;
    if(xMax < data.X1)
      xMax = data.X1;
    if(yMin > data.X2)
      yMin = data.X2;
    if(yMax < data.X2)
      yMax = data.X2;
    if(data.Origin == 1)
    {
      if(!flagNN || data.Prediction == data.Origin)
        grSig->SetPoint(grSig->GetN(), data.X1, data.X2);
      else
        grSigWrong->SetPoint(grSigWrong->GetN(), data.X1, data.X2);
    }
    else
    {
      if(!flagNN || data.Prediction == data.Origin)
        grBkg->SetPoint(grBkg->GetN(), data.X1, data.X2);
      else
        grBkgWrong->SetPoint(grBkgWrong->GetN(), data.X1, data.X2);
    }
  }
  TCanvas* c = new TCanvas("", "", 600, 600);
  TH2D* hr = new TH2D("", "", 1, xMin, xMax, 1, yMin, yMax);
  hr->Draw();
  if(f)
  {
    f->SetNpx(250);
    f->SetNpy(250);
    f->Draw("col");
  }
  grSig->Draw("p");
  grBkg->Draw("p");
  if(flagNN)
  {
    grSigWrong->Draw("p");
    grBkgWrong->Draw("p");
  }
  //if(grBorder)
  //  grBorder->Draw("c");
  c->SaveAs((fileName + std::string(".pdf")).c_str());
  c->SaveAs((fileName + std::string(".png")).c_str());
}

void ZData::DrawNNY(const std::vector<ZData>* vData, const std::string& fileName)
{
  gStyle->SetOptStat(0000000000);
  TH1D* hSig = new TH1D("", "", 100, -1.0, 1.0);
  hSig->SetLineColor(2);
  hSig->SetLineWidth(2.0);
  TH1D* hBkg = new TH1D("", "", 100, -1.0, 1.0);
  hBkg->SetLineColor(4);
  hBkg->SetLineWidth(2.0);
  for(size_t i = 0; i < vData->size(); i++)
  {
    const ZData& data = (*vData)[i];
    if(data.Origin == 1)
      hSig->Fill(data.Y);
    else
      hBkg->Fill(data.Y);
  }
  TCanvas* c = new TCanvas("", "", 600, 600);
  hSig->Draw("hist");
  hBkg->Draw("hist same");
  c->SaveAs((fileName + std::string(".pdf")).c_str());
  c->SaveAs((fileName + std::string(".png")).c_str());
}
