#include "IlluminationAssessment.h"

IlluminationAssessment::IlluminationAssessment(std::string imagePath)
{
}
IlluminationAssessment::IlluminationAssessment(std::vector<std::vector<std::string>> &data)
{
	namesAndQualityOfImages = data;
	setIlluminationClassifierData();
}

IlluminationAssessment::~IlluminationAssessment()
{
}

void IlluminationAssessment::setIlluminationClassifierData()
{
	imagePool = namesAndQualityOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		// Sets 3 quality numbers to vector
		gradesOfImages.push_back(getIlluminationMeasures(namesAndQualityOfImages[i][o.path]));
		//std::cout << "read: " << namesAndQualityOfImages[i][0] << std::endl; // OUTPUT IS FOR DEBUGGING
	}
}

std::vector<float> IlluminationAssessment::getIlluminationMeasures(std::string &path)
{
	cv::Mat imgOriginal;
	cv::Mat imgBlurred;
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgBlurred = SingletonUtilities::applyMovingAverageFilter(&imgOriginal, 3);
	

	std::vector<float> illuminationMeasures;
	// See documentation - Illumination assessment algorithm flowchart
	float IM1 = 0, IM2=0, IM3=0, IM4;

	for (int x = 0; x < imgBlurred.cols; x++) {
		for (int y = 0; y < imgBlurred.rows; y++) {
			cv::Vec3b illumination = imgBlurred.at<cv::Vec3b>(cv::Point(x, y));
			int cmin = min(illumination.val[0], min(illumination.val[1], illumination.val[2]));
			int cmax = max(illumination.val[0], max(illumination.val[1], illumination.val[2]));
			float ill = (cmin + cmax) / 2;
			IM1 += ill;
		}
	}
	IM1 /= imgBlurred.total();

	for (int x = 0; x < imgBlurred.cols; x++) {
		for (int y = 0; y < imgBlurred.rows; y++) {
			cv::Vec3b illumination = imgBlurred.at<cv::Vec3b>(cv::Point(x, y));
			int cmin = min(illumination.val[0], min(illumination.val[1], illumination.val[2]));
			int cmax = max(illumination.val[0], max(illumination.val[1], illumination.val[2]));
			float ill = (cmin + cmax) / 2;
			if (ill < IM1) IM2++;
			else if (ill > IM1) IM3++;
		}
	}

	IM4 = imgBlurred.total();
	 
	illuminationMeasures.push_back(IM1);
	illuminationMeasures.push_back(IM2);
	illuminationMeasures.push_back(IM3);
	illuminationMeasures.push_back(IM4);

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
	return illuminationMeasures;
}

std::vector<float> IlluminationAssessment::GetIlluminationQuality(std::string path) // FIX ME
{
	std::vector<float> illuminationMeasures = getIlluminationMeasures(path);

	return illuminationMeasures;
}