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

	// SHOULD USE COLORMAP (APPLYCOLORMAP)
	//cv::Mat imgColorMap;
	//cv::applyColorMap(imgOriginal, imgColorMap, cv::COLORMAP_AUTUMN);
	//SingletonUtilities::Instance()->DisplayImage("Colormap", &imgColorMap);

	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	// Should be a colormap image
	// Should have 1 dimension
	cv::cvtColor(imgOriginal, imgColorMap, CV_BGR2GRAY);
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgColorMap);

	imgBlurred = SingletonUtilities::Instance()->ApplyMovingAverageFilter(&imgCropped, 3);
}

void ContrastAssesment::applyColorMap()
{
	cv::Mat imgOriginal;
	cv::Mat imgColorMap;

	imgOriginal = SingletonUtilities::Instance()->ReadImage("images/image_girl.jpg");
	// Applies standard COLORMAP_AUTUMN
	cv::applyColorMap(imgOriginal, imgColorMap, cv::COLORMAP_AUTUMN);
}

// DOES STUFF

cv::Mat imgBlurred;
blur(imgColorMap, imgBlurred, cv::Size(3, 3));
SingletonUtilities::Instance()->DisplayImage("Blurred", &imgBlurred);

// KIEKVIENAM PASKAICIUOTI 255
// PADARYTI AISKIAU
std::vector<int> pixelCount(256); // initializes with 0
std::vector<float> pixelPercentageWithinBin(16);
float sum = 0;
int totalPixels = imgBlurred.total();
float percentage = 0;

// Go through whole matrix and see if there is this bin
for (int x = 0; x < imgBlurred.rows; x++)
{
	for (int y = 0; y < imgBlurred.cols; y++)
	{
		//pixelCount[];
		pixelCount[(int)imgBlurred.at<uchar>(x, y)]++;
		//std::cout << "pixel: " << (int)imgBlurred.at<uchar>(x, y) << std::endl;
	}
}

for (int i = 0; i < 16; i++)
{
	for (int j = 0; j < 16; j++)
	{
		pixelPercentageWithinBin[i] += pixelCount[i * 16 + j];
	}
	sum += pixelPercentageWithinBin[i];
	pixelPercentageWithinBin[i] = (float)pixelPercentageWithinBin[i] / imgBlurred.total() * 100;

	percentage += pixelPercentageWithinBin[i];
	std::cout << "Pixel percentage = " << pixelPercentageWithinBin[i] << std::endl;
}
std::cout << "Total = " << imgBlurred.total() << std::endl;
std::cout << "Total sum = " << sum << std::endl;
std::cout << "Total percent = " << percentage << std::endl;

float idealPercentage = 6.25;
float CtM1 = 0;
int CtM2 = 0;

for (int i = 0; i < 16; i++)
{
	CtM1 += abs(pixelPercentageWithinBin[i] - idealPercentage);
	if (pixelPercentageWithinBin[i] == 0)
	{
		CtM2++;
	}
}
std::cout << "CtM1 = " << CtM1 << std::endl;
std::cout << "CtM2 = " << CtM2 << std::endl;*/