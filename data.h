#ifndef ZDATA_H
#define ZDATA_H

#include <vector>
#include <string>

class ZData
{
  public:
    ZData();
    double X1;
    double X2;
    double Y;
    int Origin; // 0 bkg, 1 sig
    int Type; // 0 train, 1 validation

    static constexpr double MeanSigX1 = 1.0;
    static constexpr double MeanSigX2 = 1.0;
    static constexpr double MeanBkgX1 = -1.0;
    static constexpr double MeanBkgX2 = -1.0;
    static constexpr double RMS = 1.0;

    static std::vector<ZData>* GenerateData(const int n, const double fractionSignal = 0.5, const double fractionValid = 0.5);
    static void DrawData(const std::vector<ZData>* vData, const std::string& fileName);
    static void DrawNNY(const std::vector<ZData>* vData, const std::string& fileName);
};

#endif // ZDATA_H
