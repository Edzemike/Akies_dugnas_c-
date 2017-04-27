#include "ContrastAssesment.h"

/*ContrastAssesment::ContrastAssesment()
{
}

ContrastAssesment::ContrastAssesment(std::vector<std::vector<std::string>> &data)
{
	namesAndQualityOfImages = data;
	setContrastClassifierData();
}

ContrastAssesment::~ContrastAssesment()
{
}

void ContrastAssesment::setContrastClassifierData()
{

}

std::vector<float> ContrastAssesment::GetContrastQuality(std::string path)
{
	// Get given image's contrast measures
	std::vector<float> contrastMeasures = getContrastMeasures(path);
	return contrastMeasures;
}

std::vector<float> ContrastAssesment::getContrastMeasures(std::string &path)
{
	cv::Mat imgOriginal;
	cv::Mat imgColorMap;
	cv::Mat imgCropped;
	cv::Mat imgBlurred;
	float idealPercentage = 6.25;
	float CtM1 = 0;
	float CtM4 = 0;
	int CtM2 = 0;
	int CtM3 = 0;
	// Size of bin
	int bin = 16;
	int totalPixels = imgBlurred.total();
	std::vector<int> pixelCount(256); // initializes with 0
	std::vector<float> pixelPercentageWithinBin(bin);
	std::vector<float> contrastMeasures;

	// SHOULD USE COLORMAP (APPLYCOLORMAP)
	//cv::Mat imgColorMap;
	//cv::applyColorMap(imgOriginal, imgColorMap, cv::COLORMAP_AUTUMN);
	//SingletonUtilities::Instance()->DisplayImage("Colormap", &imgColorMap);

	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	// Should be a colormap image
	// Should have 1 dimension
	cv::cvtColor(imgOriginal, imgColorMap, CV_BGR2GRAY); // Grayscale. Should be ColorMap function.
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgColorMap);

	// Count with original image
	getPixelCount(&imgCropped, pixelCount);
	setPixelPercentageWithinBin(&bin, &totalPixels, pixelPercentageWithinBin, pixelCount);
	for (int i = 0; i < bin; i++)
	{
		CtM1 += abs(pixelPercentageWithinBin[i] - idealPercentage);
		if (pixelPercentageWithinBin[i] == 0)
		{
			CtM2++;
		}
	}
	contrastMeasures.push_back(CtM1);
	contrastMeasures.push_back(CtM2);

	// Count with blurred image
	imgBlurred = SingletonUtilities::Instance()->ApplyMovingAverageFilter(&imgCropped, 3);
	getPixelCount(&imgBlurred, pixelCount);
	setPixelPercentageWithinBin(&bin, &totalPixels, pixelPercentageWithinBin, pixelCount);
	for (int i = 0; i < bin; i++)
	{
		CtM4 += abs(pixelPercentageWithinBin[i] - idealPercentage);
		if (pixelPercentageWithinBin[i] == 0)
		{
			CtM3++;
		}
	}
	contrastMeasures.push_back(CtM3);
	contrastMeasures.push_back(CtM4);

	return contrastMeasures;
}

void ContrastAssesment::getPixelCount(cv::Mat *imgSource, std::vector<int> &pixelCount)
{
	// Go through whole matrix
	for (int x = 0; x < imgSource->rows; x++)
	{
		for (int y = 0; y < imgSource->cols; y++)
		{
			// Count how many pixels are of this intensity
			pixelCount[(int)imgSource->at<uchar>(x, y)]++;
		}
	}
}

void ContrastAssesment::setPixelPercentageWithinBin(int* bin, int *totalPixels, std::vector<float> &pixelPercentage, std::vector<int> &pixelCount)
{
	for (int i = 0; i < *bin; i++)
	{
		// Go through each pixel of bin
		for (int j = 0; j < *bin; j++)
		{
			// Compute sum of pixels in this bin
			pixelPercentage[i] += pixelCount[i * *bin + j];
		}
		pixelPercentage[i] = (float)pixelPercentage[i] / *totalPixels * 100;
	}
}

void ContrastAssesment::applyColorMap()
{
	cv::Mat imgOriginal;
	cv::Mat imgColorMap;

	imgOriginal = SingletonUtilities::Instance()->ReadImage("images/image_girl.jpg");
	// Applies standard COLORMAP_AUTUMN
	cv::applyColorMap(imgOriginal, imgColorMap, cv::COLORMAP_AUTUMN);
}*/