#include "ContrastAssesment.h"

ContrastAssesment::ContrastAssesment()
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
	imagePool = namesAndQualityOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		// Sets 4 quality numbers to vector
		gradesOfImages.push_back(getContrastMeasures(namesAndQualityOfImages[i][o.path]));
		std::cout << "read: " << namesAndQualityOfImages[i][0] << std::endl; // OUTPUT IS FOR DEBUGGING
		//std::cout << "measures: " << gradesOfImages[i][0] << " " << gradesOfImages[i][1] << " " << gradesOfImages[i][2] << " " << gradesOfImages[i][3] << " " << std::endl; // OUTPUT IS FOR DEBUGGING
		//std::cout << "diff: " << abs(gradesOfImages[i][0] - gradesOfImages[i][3]) << std::endl;
	}
}

std::vector<float> ContrastAssesment::GetContrastQuality(std::string path)
{
	std::vector<std::vector<float>> distances;

	// Get given image's contrast measures
	std::vector<float> contrastMeasures = getContrastMeasures(path);

	// Sets the distances
	setDistancesFromOriginal(distances, contrastMeasures);

	// Sort the distances vector
	std::sort(distances.begin(), distances.end(), [](const std::vector<float>& a, const std::vector<float>& b) { return a[0] < b[0]; });

	// Searches in 5 nearest images and push_back to contrastMeasures the answer (quality of image)
	mostMatchesInNearest(distances, contrastMeasures, 5);

	return contrastMeasures;
}

// MAKE IT WORK
void ContrastAssesment::mostMatchesInNearest(std::vector<std::vector<float>> &distances, std::vector<float> &focusMeasures, int nearestPool) // FIX ME
{
	int count_good = 0;
	int count_normal = 0;
	int count_bad = 0;
	// Most matches from image groups in nearest dots (nearestPool)
	for (int i = 0; i < nearestPool; i++)
	{
		if (distances[i][o.quality] == o.good)
		{
			count_good++;
			continue;
		}
		if (distances[i][o.quality] == o.normal)
		{
			count_normal++;
			continue;
		}
		if (distances[i][o.quality] == o.bad)
		{
			count_bad++;
		}
	}

	// CAN BE OPTIMIZED
	if (count_good > count_bad && count_good > count_normal)
	{
		focusMeasures.push_back(o.good);
	}
	else if (count_normal > count_good && count_normal > count_bad)
	{
		focusMeasures.push_back(o.normal);
	}
	else if (count_bad > count_good && count_bad > count_normal)
	{
		focusMeasures.push_back(o.bad);
	}
	else
	{
		std::cout << "error: two classifiers are at the same distance\n";
		_getch();
		exit(EXIT_FAILURE);
	}
}

void ContrastAssesment::setDistancesFromOriginal(std::vector<std::vector<float>> &distances, std::vector<float> &original)
{
	// Goes through all pictures
	for (int i = 0; i < imagePool; i++)
	{
		//if (original[o.CtM2] == original[o.CtM3])
		//{
			// distance = (x11-x21)^2+(y12-y22)^2
		distances.push_back({
			pow((original[o.CtM1] - gradesOfImages[i][o.CtM1]), 2) +
			pow((original[o.CtM4] - gradesOfImages[i][o.CtM4]), 2) });
		//}
		//else
		//{
			// If CtM2 != CtM3 focus is bad, so set distances to 0
			//std::fill(distances[i].begin(), distances[i].end(), 0);
		//}

		if (namesAndQualityOfImages[i][o.quality] == "good")
			distances[i].push_back(o.good);
		else if (namesAndQualityOfImages[i][o.quality] == "normal")
			distances[i].push_back(o.normal);
		else if (namesAndQualityOfImages[i][o.quality] == "bad")
			distances[i].push_back(o.bad);
	}
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
	int totalPixels;
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
	totalPixels = imgCropped.total();

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
	// Set pixelCount to zero
	std::fill(pixelCount.begin(), pixelCount.end(), 0);
	getPixelCount(&imgBlurred, pixelCount);
	// Set pixelPercentageWithinBin to zero
	std::fill(pixelPercentageWithinBin.begin(), pixelPercentageWithinBin.end(), 0);
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
}