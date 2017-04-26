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

class IlluminationAssessment {
public:
	IlluminationAssessment(std::string);
	IlluminationAssessment(std::vector<std::vector<std::string>>&);
	~IlluminationAssessment();
	std::vector<float> GetIlluminationQuality(std::string);

private:
	int imagePool;
	std::vector<std::vector<std::string>> namesAndQualityOfImages;
	std::vector<std::vector<float>> gradesOfImages;

	void setIlluminationClassifierData();
	std::vector<float> getIlluminationMeasures(std::string&);
};