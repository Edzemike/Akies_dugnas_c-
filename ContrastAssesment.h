#pragma once

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
	std::vector<std::vector<std::string>> namesAndQualityOfImages;
	std::vector<std::vector<float>> gradesOfImages;

	void setContrastClassifierData();
	void applyColorMap();
	void setPixelPercentageWithinBin(int*, int*, std::vector<float>&, std::vector<int>&);
	void getPixelCount(cv::Mat*, std::vector<int>&);
	std::vector<float> getContrastMeasures(std::string&);
};