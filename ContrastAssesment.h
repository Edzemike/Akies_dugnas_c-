#pragma once
#include <fstream>

#include "SingletonUtilities.h"

class ContrastAssesment
{
public:
	ContrastAssesment();
	ContrastAssesment(std::vector<std::vector<std::string>>&);
	~ContrastAssesment();
	std::vector<float> GetContrastQuality(std::string);

private:
	int imagePool;
	std::vector<std::vector<std::string>> namesAndContrastOfImages;
	std::vector<std::vector<float>> gradesOfImages;

	void setContrastClassifierData();
	void getContrastClassifierData();
	void applyColorMap();
	void setPixelPercentageWithinBin(int*, int*, std::vector<float>&, std::vector<int>&);
	void getPixelCount(cv::Mat*, std::vector<int>&);
	void setDistancesFromOriginal(std::vector<std::vector<float>>&, std::vector<float>&);
	void mostMatchesInNearest(std::vector<std::vector<float>>&, std::vector<float>&, int);
	std::vector<float> getContrastMeasures(std::string&);
};