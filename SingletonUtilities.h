#pragma once

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

// Object to help with iterators (easier to read code)
static struct o
{
	// Contrast
	const int low = 0;
	const int high = 1;
	// Quality
	const int good = 0;
	const int normal = 1;
	const int bad = 2;
	// To the picture
	const int path = 0;
	// Of the picture
	const int contrast = 1;
	const int quality = 1;
	// Focus measures
	const int FM1 = 0;
	const int FM2 = 1;
	const int FM3 = 2;
	// Distance from original
	const int distance = 0;
	// Contrast measures
	const int CtM1 = 0;
	const int CtM2 = 1;
	const int CtM3 = 2;
	const int CtM4 = 3;
} o;

class SingletonUtilities
{
public:
	SingletonUtilities();
	~SingletonUtilities();
	static SingletonUtilities* Instance();
	std::vector<std::string> GetFilesNamesInFolder(std::string folder);
	void SaveImage(std::string, cv::Mat*);
	void SaveBackProjection(std::string, cv::Mat*);
	void DisplayImage(std::string, cv::Mat*);
	cv::Mat ApplyColorMap(cv::Mat, cv::Mat);
	cv::Mat ReadImage(std::string);
	cv::Mat ApplyMovingAverageFilter(cv::Mat *imgGrayscale, int filterSize);
	cv::Mat CropToROI(cv::Mat*);

private:
	int imagePool; // Don't mess this up (can be 1-10) helps to read images
	static SingletonUtilities *utilities_instance;
};