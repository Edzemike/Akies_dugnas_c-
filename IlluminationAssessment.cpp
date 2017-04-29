#include "IlluminationAssessment.h"

IlluminationAssessment::IlluminationAssessment(std::string imagePath)
{
}
IlluminationAssessment::IlluminationAssessment(std::vector<std::string> &data)
{
	namesOfImages = data;
	setIlluminationClassifierData();
}

IlluminationAssessment::~IlluminationAssessment()
{
}

void IlluminationAssessment::setIlluminationClassifierData()
{
	imagePool = namesOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		// Sets 3 quality numbers to vector
		gradesOfImages.push_back(getIlluminationMeasuresBGR("./images/Illumination/" + namesOfImages[i]));
		std::cout << "read: " << namesOfImages[i] << std::endl; // OUTPUT IS FOR DEBUGGING
		std::cout << gradesOfImages[i][0] << " " << gradesOfImages[i][1] << " " << gradesOfImages[i][2] << " " << gradesOfImages[i][3] << " " << std::endl;

	}
}

std::vector<float> IlluminationAssessment::getIlluminationMeasuresBGR(std::string &path)
{
	cv::Mat imgOriginal;
	cv::Mat imgBlurred;
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgBlurred = SingletonUtilities::Instance()->ApplyMovingAverageFilter(&imgOriginal, 3);
	

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
	float sumIM2 = 0, sumIM3 = 0, sumIM4 = 0;
	for (int x = 0; x < imgBlurred.cols; x++) {
		for (int y = 0; y < imgBlurred.rows; y++) {
			cv::Vec3b illumination = imgBlurred.at<cv::Vec3b>(cv::Point(x, y));
			int cmin = min(illumination.val[0], min(illumination.val[1], illumination.val[2]));
			int cmax = max(illumination.val[0], max(illumination.val[1], illumination.val[2]));
			float ill = (cmin + cmax) / 2;
			if (ill < IM1) {
				IM2++;
				sumIM2 += pow((ill - IM1), 2);
			}
			else if (ill > IM1) {
				IM3++;
				sumIM3 += pow((ill - IM1), 2);
			}
			sumIM4 += pow((ill - IM1), 2);
		}
	}

	IM2 = sqrt(sumIM2 / (IM2 - 1));
	IM3 = sqrt(sumIM3 / (IM3 - 1));
	IM4 = sqrt(sumIM4 / (imgBlurred.total() - 1));
	 
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

std::vector<float> IlluminationAssessment::getIlluminationMeasuresHSL(std::string &path)
{ // change whole method
	cv::Mat imgOriginal;
	cv::Mat imgBlurred;
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgBlurred = SingletonUtilities::Instance()->ApplyMovingAverageFilter(&imgOriginal, 3);


	std::vector<float> illuminationMeasures;
	// See documentation - Illumination assessment algorithm flowchart
	float IM1 = 0, IM2 = 0, IM3 = 0, IM4;

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
	std::vector<float> illuminationMeasures = getIlluminationMeasuresBGR(path);

	return illuminationMeasures;
}