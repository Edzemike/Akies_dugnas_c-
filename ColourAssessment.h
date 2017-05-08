#pragma once
#include "SingletonUtilities.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <math.h> // pow
#include <functional> // less
#include <vector>
#include <Windows.h> // readImages
#include <algorithm> // transform

class ColourAssessment {
public:
	ColourAssessment(std::string);
	ColourAssessment(std::vector<std::string>&);
	~ColourAssessment();
	//std::vector<float> GetColourQuality(std::string, int alg);
	std::vector<float> getColourMeasuresBGR(std::string); // was private
	std::vector<float> getColourMeasuresHSV(std::string);
	std::string getColourQuality(std::vector<float> mesurements);
	void makeBackProjections(std::string path);
private:
	int imagePool;
	int Bmax = 255, Omax = 170, Dmax = 85;
	void ColourAssessment::setColourClassifierData(std::vector<std::string> namesOfImages);
	std::vector<std::string> namesOfImages;
	std::vector<std::vector<float>> gradesOfImages;
//	void setColourClassifierData();
};