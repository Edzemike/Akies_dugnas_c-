#include "FocusAssessment.h"

FocusAssessment::FocusAssessment(std::string imagePath)
/**
* Tuðèias konstruktorius
*/
{
}

FocusAssessment::FocusAssessment(std::vector<std::vector<std::string>> &data)
/**
* Konstruktorius, kuris automatiškai apsimokina (visada naudojamas).
*/
{
	namesAndQualityOfImages = data;
	setFocusClassifierData();
	getFocusClassifierData();
}

FocusAssessment::~FocusAssessment()
/**
* Destruktorius.
*/
{
}

cv::Mat FocusAssessment::convertToGrayscale(cv::Mat *imgOriginal)
/**
* Paverčia BGR tipo paveikslėlį į pilką ir jį grąžina.
*/
{
	cv::Mat imgGrayscale;
	//				from		   to
	cv::cvtColor(*imgOriginal, imgGrayscale, CV_BGR2GRAY);
	return imgGrayscale;
}

double FocusAssessment::getGradientMean(cv::Mat *imgGradient)
/**
* Apskaičiuoja matricos vidurkį ir jį grąžina.
*/
{
	cv::Scalar meanValue = cv::mean(*imgGradient);
	return meanValue[0];
}

cv::Mat FocusAssessment::applySobelOperator(cv::Mat *imgGrayscale) // Sobel operation makes gradient map (O)
/**
* Pritaiko Sobelio filtrą ir grąžina matricą.
*/
{
	cv::Mat imgSobel;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	/// Generate grad_x and grad_y
	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	Sobel(*imgGrayscale, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	Sobel(*imgGrayscale, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, imgSobel);

	return imgSobel;
}

std::vector<float> FocusAssessment::getFocusMeasures(std::string &path)
/**
* Įvykdo dokumentacijoje aprašytą fokuso algoritmą ir
* grąžina nuotraukos įverčius vektoriuje.
*/
{
	cv::Mat imgOriginal;
	cv::Mat imgGrayscale;
	cv::Mat imgCropped;
	cv::Mat imgBlurred;
	cv::Mat imgGradient;

	std::vector<float> focusMeasures;
	//float* focusMeasures = new float[3];
	// See documentation - focus assessment algorithm flowchart
	float FM1, FM2, FM3, fm2, fm3;

	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgOriginal);
	imgGrayscale = convertToGrayscale(&imgCropped);

	imgGradient = applySobelOperator(&imgGrayscale);
	FM1 = getGradientMean(&imgGradient);
	focusMeasures.push_back(FM1);

	imgBlurred = SingletonUtilities::Instance()->ApplyMovingAverageFilter(&imgGrayscale, 3);
	imgGradient = applySobelOperator(&imgBlurred);
	fm2 = getGradientMean(&imgGradient);
	FM2 = FM1 - fm2;
	focusMeasures.push_back(FM2);

	imgBlurred = SingletonUtilities::Instance()->ApplyMovingAverageFilter(&imgGrayscale, 5);
	imgGradient = applySobelOperator(&imgBlurred);
	fm3 = getGradientMean(&imgGradient);
	FM3 = fm2 - fm3;
	focusMeasures.push_back(FM3);

	// In case you need to save an image
	//srand(time(0)); // Seed random name generator
	//SingletonUtilities::Instance()->SaveImage("images/", &imgCropped);

	//SingletonUtilities::Instance()->DisplayImage("imgOriginal", &imgOriginal); // THIS IS FOR DEBUGGING
	// Output
	/*std::printf("FM1: %f\nFM2: %f\nFM3: %f\n", FM1, FM2, FM3);
	SingletonUtilities::Instance()->DisplayImage("imgCropped", &imgCropped);
	SingletonUtilities::Instance()->DisplayImage("imgOriginal", &imgOriginal);
	SingletonUtilities::Instance()->DisplayImage("imgGradient", &imgGradient);
	SingletonUtilities::Instance()->DisplayImage("imgBlurred", &imgBlurred);*/
	return focusMeasures;
}

void FocusAssessment::setFocusClassifierData()
/**
* Apsimokinimo metodas. Įrašo nuotraukos įverčius į failą.
*/
{
	/*imagePool = namesAndQualityOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		// Sets 3 quality numbers to vector
		gradesOfImages.push_back(getFocusMeasures(namesAndQualityOfImages[i][o.path]));
		//std::cout << "read: " << namesAndQualityOfImages[i][0] << std::endl; // OUTPUT IS FOR DEBUGGING
	}*/
	std::ofstream output;
	output.open("./qualityParameters/parametersFocus.txt");
	imagePool = namesAndQualityOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		gradesOfImages.push_back(getFocusMeasures(namesAndQualityOfImages[i][o.path]));
		//std::cout << "read: " << namesAndQualityOfImages[i][0] << std::endl; // OUTPUT IS FOR DEBUGGING
		output << gradesOfImages[i][o.FM1] << " " << gradesOfImages[i][o.FM2] << " " << gradesOfImages[i][o.FM3] << std::endl;
	}
	output.close();
}

void FocusAssessment::getFocusClassifierData()
/**
* Nuskaitymo metodas. Įrašo nuotraukos įverčius į vektorių.
*/
{
	std::ifstream parameters;
	float FM1;
	float FM2;
	float FM3;
	parameters.open("./qualityParameters/parametersFocus.txt");
	while (!parameters.eof())
	{
		parameters >> FM1;
		parameters >> FM2;
		parameters >> FM3;
		gradesOfImages.push_back({FM1, FM2, FM3});
	}
	parameters.close();
}

void FocusAssessment::mostMatchesInNearest(std::vector<std::vector<float>> &distances, std::vector<float> &focusMeasures, int nearestPool)
/**
* Metodas, kuris apskaičiuoja žodinį nuotraukos įvertį
* pagal didžiausią kiekį iš artimiausių nearestPool.
*/
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
		std::cout << "error: two classifiers are at the same distance (focus)\n";
		_getch();
		exit(EXIT_FAILURE);
	}
}

void FocusAssessment::setDistancesFromOriginal(std::vector<std::vector<float>> &distances, std::vector<float> &original) // Rodykles i kintamuosius
/**
* Metodas paskaičiuoja apsimokinimo nuotraukų atstumus
* nuo paduotos nuotraukos. Taip pat įrašo žodinį nuotraukos
* įvertį.
*/
{
	// Goes through all pictures
	for (int i = 0; i < imagePool; i++)
	{
		// distance = (x11-x21)^2+(y12-y22)^2+(z13-z23)^2
		distances.push_back({
			pow((original[o.FM1] - gradesOfImages[i][o.FM1]), 2) +
			pow((original[o.FM2] - gradesOfImages[i][o.FM2]), 2) +
			pow((original[o.FM3] - gradesOfImages[i][o.FM3]), 2) });

		if (namesAndQualityOfImages[i][o.quality] == "good")
			distances[i].push_back(o.good);
		else if (namesAndQualityOfImages[i][o.quality] == "normal")
			distances[i].push_back(o.normal);
		else if (namesAndQualityOfImages[i][o.quality] == "bad")
			distances[i].push_back(o.bad);
	}
}

std::vector<float> FocusAssessment::GetFocusQuality(std::string path)
/**
* Metodas, kuris apskaičiuoja duotos nuotraukos įverčius
* priklausomai nuo apsimokinimo duomenų ir grąžina rezultatą
* kartu su žodiniu įverčiu.
*/
{
	std::vector<std::vector<float>> distances;

	// Get given image's focus measures
	std::vector<float> focusMeasures = getFocusMeasures(path);

	// Sets the distances
	setDistancesFromOriginal(distances, focusMeasures);

	// Sort the distances vector
	std::sort(distances.begin(), distances.end(), [](const std::vector<float>& a, const std::vector<float>& b) { return a[0] < b[0]; });

	// Searches in 5 nearest images and push_back to focusMeasures the answer (quality of image)
	mostMatchesInNearest(distances, focusMeasures, 5);

	return focusMeasures;
}