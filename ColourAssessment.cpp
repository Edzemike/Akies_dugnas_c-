#include "ColourAssessment.h"

ColourAssessment::ColourAssessment(std::string imagePath)
{
}

ColourAssessment::ColourAssessment(std::vector<std::string> &data)
/**
* ąčęėįšųū 
*/
{
	namesOfImages = data;
	std::cout << namesOfImages[0] << namesOfImages[1] << namesOfImages[2] << std::endl; // OK
	setColourClassifierData(namesOfImages);
}

ColourAssessment::~ColourAssessment()
{
}

void ColourAssessment::setColourClassifierData(std::vector<std::string> namesOfImages)
{
	std::ofstream output;
	output.open("./qualityParameters/parametersColour.txt");
	imagePool = namesOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		// sets 3 quality numbers to vector
		gradesOfImages.push_back(getColourMeasuresHSV("./images/Colour/"+namesOfImages[i]));
		std::cout << "read: " << namesOfImages[i] << std::endl; // output is for debugging
		output << gradesOfImages[i][0] << " " << gradesOfImages[i][1] << " " << gradesOfImages[i][2] << " " << namesOfImages[i] << std::endl;
	}
	output.close();
}

std::vector<float> ColourAssessment::getColourMeasuresBGR(std::string &path)
{
	cv::Mat imgOriginal;
	std::vector<float> ColourMeasures;
	float CM1, CM2, CM3;
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	int B = 0, D = 0, O = 0; // brigth dark normal
	int totalPixels = imgOriginal.total();

	for (int x = 0; x < imgOriginal.cols; x++) {
		for (int y = 0; y < imgOriginal.rows; y++) {
			cv::Vec3b colour = imgOriginal.at<cv::Vec3b>(cv::Point(x, y));
			int averageColor = (colour.val[0] + colour.val[1] + colour.val[2]) / 3;

			if (averageColor <= Bmax && averageColor > Omax)
				B++;
			else if (averageColor <= Omax && averageColor > Dmax)
				O++;
			else if (averageColor <= Dmax && averageColor >= 0)
				D++;
			else
				std::cout << "Unexpected error." << std::endl;
		}
	}

	CM1 = (float)B / (float)totalPixels;
	CM2 = (float)D / (float)totalPixels;
	CM3 = (float)O / (float)totalPixels;

	ColourMeasures.push_back(CM1);
	ColourMeasures.push_back(CM2);
	ColourMeasures.push_back(CM3);

	// In case you need to save an image
	//srand(time(0)); // Seed random name generator
	//SingletonUtilities::Instance()->SaveImage("images/", &imgCropped);

	SingletonUtilities::Instance()->DisplayImage("imgOriginal", &imgOriginal); // THIS IS FOR DEBUGGING
																			   // Output
																			   /*std::printf("CM1: %f\nCM2: %f\nCM3: %f\n", CM1, CM2, CM3);
																			   SingletonUtilities::Instance()->DisplayImage("imgCropped", &imgCropped);
																			   SingletonUtilities::Instance()->DisplayImage("imgOriginal", &imgOriginal);
																			   SingletonUtilities::Instance()->DisplayImage("imgGradient", &imgGradient);
																			   SingletonUtilities::Instance()->DisplayImage("imgBlurred", &imgBlurred);*/
	return ColourMeasures;
}

std::vector<float> ColourAssessment::getColourMeasuresHSV(std::string &path)
{

	cv::Mat imgOriginal;
	cv::Mat imgCropped;
	cv::Mat imgHSV;
	std::vector<float> ColourMeasures;
	float CM1, CM2, CM3;
	int B = 0, D = 0, O = 0; // brigth dark normal
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgOriginal);
	
	int totalPixels = imgCropped.total();
	
	cv::cvtColor(imgCropped, imgHSV, cv::COLOR_BGR2HSV);

	for (int x = 0; x < imgHSV.cols; x++) {
		for (int y = 0; y < imgHSV.rows; y++) {
			cv::Vec3b colour = imgHSV.at<cv::Vec3b>(cv::Point(x, y));
			int colorBrightness = (colour.val[2]);// + colour.val[1] + colour.val[2]) / 3;

			if (colorBrightness <= Bmax && colorBrightness > Omax)
				B++;
			else if (colorBrightness <= Omax && colorBrightness > Dmax)
				O++;
			else if (colorBrightness <= Dmax && colorBrightness >= 0)
				D++;
			else
				std::cout << "Unexpected error." << std::endl;
		}
	}

	CM1 = (float)B / (float)totalPixels;
	CM2 = (float)D / (float)totalPixels;
	CM3 = (float)O / (float)totalPixels;

	ColourMeasures.push_back(CM1);
	ColourMeasures.push_back(CM2);
	ColourMeasures.push_back(CM3);

	// In case you need to save an image
	//srand(time(0)); // Seed random name generator
	//SingletonUtilities::Instance()->SaveImage("images/", &imgCropped);

	SingletonUtilities::Instance()->DisplayImage("imgOriginal", &imgOriginal); // THIS IS FOR DEBUGGING
																			   // Output
																			   /*std::printf("CM1: %f\nCM2: %f\nCM3: %f\n", CM1, CM2, CM3);
																			   SingletonUtilities::Instance()->DisplayImage("imgCropped", &imgCropped);
																			   SingletonUtilities::Instance()->DisplayImage("imgOriginal", &imgOriginal);
																			   SingletonUtilities::Instance()->DisplayImage("imgGradient", &imgGradient);
																			   SingletonUtilities::Instance()->DisplayImage("imgBlurred", &imgBlurred);*/
	return ColourMeasures;
}

std::vector<float> ColourAssessment::GetColourQuality(std::string path, int alg) // FIX ME
{
	//std::vector<std::vector<float>> distances;

	std::vector<float> ColourMeasures;
	if( alg = 1)
			ColourMeasures = getColourMeasuresBGR(path);
	else
			ColourMeasures = getColourMeasuresHSV(path);

	// Sort the distances vector
	//std::sort(distances.begin(), distances.end(), [](const std::vector<float>& a, const std::vector<float>& b) { return a[1] > b[1]; });
	// Sets the distances
	//setDistancesFromOriginal(distances, ColourMeasures);

	// Searches in 5 nearest images and push_back to ColourMeasures the answer (quality of image)
	//mostMatchesInNearest(distances, ColourMeasures, 5);

	return ColourMeasures;
}

std::string ColourAssessment::getColourQuality(std::vector<float> mesurements)
{
	std::string quality = "please show some exaples of bright dark and normal";
	std::ifstream parameters;
	float a, b, c, l1 = 1, l2 = 1;
	std::string s;
	parameters.open("./qualityParameters/parametersColour.txt");
	while (!parameters.eof())
	{
		parameters >> a;
		parameters >> b;
		parameters >> c;
		parameters >> s;
		l2 = sqrt(pow((a - mesurements[0]), 2) + pow((b - mesurements[1]), 2) + pow((c - mesurements[2]), 2));
		std::cout << "l2: " << a << std::endl;
		if (l2 <= l1) {
			l1 = l2;
			quality = s;
		}
	}

	parameters.close();
	return quality;
}
