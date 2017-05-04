#include "ContrastAssesment.h"

ContrastAssesment::ContrastAssesment()
/**
* Tuščias konstruktorius.
*/
{
}

ContrastAssesment::ContrastAssesment(std::vector<std::vector<std::string>> &data)
/**
* Konstruktorius, kuris automatiškai apsimokina (visada naudojamas).
*/
{
	namesAndContrastOfImages = data;
	setContrastClassifierData();
	getContrastClassifierData();
}

ContrastAssesment::~ContrastAssesment()
/**
* Destruktorius.
*/
{
}

void ContrastAssesment::setContrastClassifierData()
/**
* Apsimokinimo metodas. Įrašo nuotraukos įverčius į failą.
*/
{
	/*imagePool = namesAndContrastOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		// Sets 4 quality numbers to vector
		gradesOfImages.push_back(getContrastMeasures(namesAndContrastOfImages[i][o.path]));
	}*/
	std::ofstream output;
	output.open("./qualityParameters/parametersContrast.txt");
	imagePool = namesAndContrastOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		gradesOfImages.push_back(getContrastMeasures(namesAndContrastOfImages[i][o.path]));
		//std::cout << "read: " << namesAndQualityOfImages[i][0] << std::endl; // OUTPUT IS FOR DEBUGGING
		output << gradesOfImages[i][o.CtM1] << " " << gradesOfImages[i][o.CtM2] << " " << gradesOfImages[i][o.CtM3] << " " << gradesOfImages[i][o.CtM4] << std::endl;
	}
	output.close();
}

void ContrastAssesment::getContrastClassifierData()
/**
* Nuskaitymo metodas. Įrašo nuotraukos įverčius į vektorių.
*/
{
	std::ifstream parameters;
	float CtM1;
	float CtM2;
	float CtM3;
	float CtM4;
	parameters.open("./qualityParameters/parametersFocus.txt");
	while (!parameters.eof())
	{
		parameters >> CtM1;
		parameters >> CtM2;
		parameters >> CtM3;
		parameters >> CtM4;
		gradesOfImages.push_back({ CtM1, CtM2, CtM3, CtM4 });
	}
	parameters.close();
}

std::vector<float> ContrastAssesment::GetContrastQuality(std::string path)
/**
* Metodas, kuris apskaičiuoja duotos nuotraukos įverčius
* priklausomai nuo apsimokinimo duomenų ir grąžina rezultatą
* kartu su žodiniu įverčiu.
*/
{
	std::vector<std::vector<float>> distances;

	// Get given image's contrast measures
	std::vector<float> contrastMeasures = getContrastMeasures(path);

	// Sets the distances
	setDistancesFromOriginal(distances, contrastMeasures);

	// Sort the distances vector
	std::sort(distances.begin(), distances.end(), [](const std::vector<float>& a, const std::vector<float>& b) { return a[0] < b[0]; });

	// Searches in 5 nearest images and push_back to contrastMeasures the answer (quality of image)
	mostMatchesInNearest(distances, contrastMeasures, 1);

	return contrastMeasures;
}

void ContrastAssesment::mostMatchesInNearest(std::vector<std::vector<float>> &distances, std::vector<float> &focusMeasures, int nearestPool)
/**
* Metodas, kuris apskaičiuoja žodinį nuotraukos įvertį
* pagal didžiausią kiekį iš artimiausių nearestPool.
*/
{
	int count_low = 0;
	int count_high = 0;
	// Most matches from image groups in nearest dots (nearestPool)
	for (int i = 0; i < nearestPool; i++)
	{
		if (distances[i][o.contrast] == o.low)
		{
			count_low++;
			continue;
		}
		if (distances[i][o.contrast] == o.high)
		{
			count_high++;
		}
	}

	// CAN BE OPTIMIZED
	if (count_high > count_low)
	{
		focusMeasures.push_back(o.high);
	}
	else if (count_low > count_high)
	{
		focusMeasures.push_back(o.low);
	}
	else
	{
		std::cout << "error: two classifiers are at the same distance (contrast)\n";
		_getch();
		exit(EXIT_FAILURE);
	}
}

void ContrastAssesment::setDistancesFromOriginal(std::vector<std::vector<float>> &distances, std::vector<float> &original)
/**
* Metodas paskaičiuoja apsimokinimo nuotraukų atstumus
* nuo paduotos nuotraukos. Taip pat įrašo žodinį nuotraukos
* įvertį.
*/
{
	// FIXME: is this algorithm correct?
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

		if (namesAndContrastOfImages[i][o.contrast] == "low")
			distances[i].push_back(o.low);
		else if (namesAndContrastOfImages[i][o.contrast] == "high")
			distances[i].push_back(o.high);
	}
}

std::vector<float> ContrastAssesment::getContrastMeasures(std::string &path)
/**
* Įvykdo dokumentacijoje aprašytą kontrasto algoritmą ir
* grąžina nuotraukos įverčius vektoriuje.
*/
{
	cv::Mat imgOriginal;
	cv::Mat imgHSV;
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
	//cv::Mat imgHSV;
	//cv::applyColorMap(imgOriginal, imgHSV, cv::COLORMAP_AUTUMN);
	//SingletonUtilities::Instance()->DisplayImage("Colormap", &imgHSV);

	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	// Should be a colormap image
	// Should have 1 dimension
	cv::cvtColor(imgOriginal, imgHSV, CV_BGR2HSV);
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgHSV);
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
/**
* Apskaičiuoja kiek yra pikselių kiekviename ryškume
* nuo 0 iki 255 ir įrašo į vektorių.
*/
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
/**
* Apskaičiuoja koks visų pikselių procentas yra kiekviename
* diapozone bin.
*/
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