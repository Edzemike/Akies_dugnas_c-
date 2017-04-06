#pragma once

#include "SingletonUtilities.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <conio.h>
#include <time.h>
#include <math.h> // pow
#include <functional> // less
#include <vector>
#include <Windows.h> // readImages
#include <algorithm> // transform

class FocusAssessment
{
public:
	FocusAssessment(std::string);
	FocusAssessment(std::vector<std::vector<std::string>>&);
	~FocusAssessment();
	std::vector<float> GetFocusQuality(std::string);

private:
	int imagePool;
	std::vector<std::vector<std::string>> namesAndQualityOfImages;
	std::vector<std::vector<float>> gradesOfImages;

	void setFocusClassifierData();
	void mostMatchesInNearest(std::vector<std::vector<float>>&, std::vector<float>&, int);
	void setDistancesFromOriginal(std::vector<std::vector<float>>&, std::vector<float>&);
	std::vector<float> getFocusMeasures(std::string&);
	double getGradientMean(cv::Mat*);
	cv::Mat applyMovingAverageFilter(cv::Mat*, int);
	//cv::Mat readImage(std::string);
	cv::Mat convertToGrayscale(cv::Mat*);
	cv::Mat applySobelOperator(cv::Mat*);
	cv::Mat cropToROI(cv::Mat*);
};