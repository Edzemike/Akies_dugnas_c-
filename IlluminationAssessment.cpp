#include "IlluminationAssessment.h"

IlluminationAssessment::IlluminationAssessment(std::string imagePath)
{
}
IlluminationAssessment::IlluminationAssessment(std::vector<std::vector<std::string>> &data)
{
	namesAndQualityOfImages = data;
	setIlluminationClassifierData();
}

void IlluminationAssessment::setIlluminationClassifierData()
{
	imagePool = namesAndQualityOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		// Sets 3 quality numbers to vector
		gradesOfImages.push_back(getIlluminationMeasures(namesAndQualityOfImages[i][o.path]));
		std::cout << "read: " << namesAndQualityOfImages[i][0] << std::endl; // OUTPUT IS FOR DEBUGGING
	}
}

std::vector<float> IlluminationAssessment::getIlluminationMeasures(std::string &path)
{
	cv::Mat imgOriginal;
	/*cv::Mat imgGrayscale;
	cv::Mat imgCropped;
	cv::Mat imgBlurred;
	cv::Mat imgGradient;*/

	std::vector<float> IlluminationMeasures;
	//float* IlluminationMeasures = new float[3];
	// See documentation - Illumination assessment algorithm flowchart
	float CM1, CM2, CM3;

	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	//imgGrayscale = convertToGrayscale(&imgOriginal);
	//imgCropped = cropToROI(&imgGrayscale);

	//imgGradient = applySobelOperator(&imgCropped);
	//CM1 = getGradientMean(&imgGradient);
	IlluminationMeasures.push_back(CM1);

	//imgBlurred = applyMovingAverageFilter(&imgCropped, 3);
	//imgGradient = applySobelOperator(&imgBlurred);
	//CM2 = getGradientMean(&imgGradient);
	IlluminationMeasures.push_back(CM2);

	//imgBlurred = applyMovingAverageFilter(&imgCropped, 5);
	//imgGradient = applySobelOperator(&imgBlurred);
	//CM3 = getGradientMean(&imgGradient);
	IlluminationMeasures.push_back(CM3);

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
	return IlluminationMeasures;
}