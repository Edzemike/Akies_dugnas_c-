#include "FocusAssessment.h"
#include "SingletonUtilities.h"
#include "ColourAssessment.h"
#include "IlluminationAssessment.h"
#include "ContrastAssesment.h"

void SetImagesNamesAndQualityFocus(std::vector<std::vector<std::string>>*);
void SetImagesNamesAndContrast(std::vector<std::vector<std::string>>*);

int main()
{
	std::vector<std::string> sourceImages = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Source");
	for (int i = 0; i < sourceImages.size(); i++)
	{
		sourceImages[i] = "images/Source/" + sourceImages[i];
	}
	std::cout << "Source OK" << std::endl;
	// Contrast variables
	cv::Mat imgColorMap;
	cv::Mat imgBackProjection;
	imgColorMap = SingletonUtilities::Instance()->ReadImage("images/Contrast/colormap/high.jpg");
	std::vector<std::vector<std::string>> namesAndContrast;
	SetImagesNamesAndContrast(&namesAndContrast);
	ContrastAssesment* objContrastAssessment = new ContrastAssesment(namesAndContrast);
	std::cout << "Contrast OK" << std::endl;
	// Focus variables
	std::vector<std::vector<std::string>> namesAndQuality;
	SetImagesNamesAndQualityFocus(&namesAndQuality);
	FocusAssessment* objFocusAssessment = new FocusAssessment(namesAndQuality);
	std::cout << "Focus OK" << std::endl;
	// Color variables
	std::vector<std::string> imageNamesColour = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Colour");
	ColourAssessment* objColourAssessment = new ColourAssessment(imageNamesColour);
	std::cout << "Color OK" << std::endl;
	// Illumination variables
	std::vector<std::string> imageNamesIllumination = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Illumination");
	IlluminationAssessment* objIlluminationAssessment = new IlluminationAssessment(imageNamesIllumination);
	std::cout << "Illumination OK" << std::endl;

	// Reads every image and outputs its' measures
	for (int i = 0; i < sourceImages.size(); i++)
	{
		std::cout << "\nQuality measures for image " << sourceImages[i] << std::endl;

		// IMPORTANT:
		// ContrastAssesment must have at least 5 photos in data folder overall.
		// Then change this line mostMatchesInNearest(distances, contrastMeasures, 1);
		// In ContrastAssesment.cpp (1 to 5)
		cv::Mat imgOriginal = SingletonUtilities::Instance()->ReadImage(sourceImages[i]);
		imgBackProjection = SingletonUtilities::Instance()->ApplyColorMap(imgOriginal, imgColorMap);
		SingletonUtilities::Instance()->SaveBackProjection("images/", &imgBackProjection);
		std::vector<float> contrastQuality = objContrastAssessment->GetContrastQuality("images/backprojection.jpg");
		//std::vector<float> contrastQuality = objContrastAssessment->GetContrastQuality(sourceImages[i]);

		std::cout << "\nContrast quality of image is ";
		contrastQuality[4] == o.high ? printf("high") : printf("low");
		std::cout << std::endl;

		std::vector<float> focusQuality = objFocusAssessment->GetFocusQuality(sourceImages[i]);

		std::cout << "Focus quality of image is ";
		focusQuality[3] == o.good ? printf("good") : focusQuality[3] == o.normal ? printf("normal") : printf("bad");
		std::cout << std::endl;

		std::vector<float> colourQuality = objColourAssessment->getColourMeasuresHSV(sourceImages[i]);
		std::cout << "Color quality mostly resembles: " << objColourAssessment->getColourQuality(colourQuality);

		std::vector<float> illuminationQuality = objIlluminationAssessment->getIlluminationMeasuresBGR(sourceImages[i]);
		std::cout << "\nIllumination quality mostly resembles: " << objIlluminationAssessment->getIlluminationQuality(illuminationQuality);

		std::cout << "\nQuality measures of image are:" << std::endl;
		std::cout << "Contrast: ";
		for (int i = 0; i < 4; i++)
		{
			std::cout << " " << contrastQuality[i];
		}
		std::cout << std::endl;
		std::cout << "Focus: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << " " << focusQuality[i];
		}
		std::cout << std::endl;
		std::cout << "Color: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << " " << colourQuality[i];
		}
		std::cout << std::endl;
		std::cout << "Illumination: ";
		for (int i = 0; i < 4; i++)
		{
			std::cout << " " << illuminationQuality[i];
		}

		std::string colorPicture = objColourAssessment->getColourQuality(colourQuality); // Remove after creating grading algorithm
		std::string illuminationPicture = objIlluminationAssessment->getIlluminationQuality(illuminationQuality); // Remove after creating grading algorithm
		// FIXME: add grading for Illumination and Color algorithms
		if (contrastQuality[4] == o.high && (focusQuality[3] == o.good || focusQuality[3] == o.normal) && colorPicture == "normal.jpg" && illuminationPicture == "even.jpg")
		{
			std::cout << "\nThis image is gradable" << std::endl;
		}
		else
		{
			std::cout << "\nThis image is ungradable" << std::endl;
		}
		std::cout << std::endl;
	}

	delete objContrastAssessment;
	delete objFocusAssessment;
	delete objColourAssessment;
	delete objIlluminationAssessment;

	//SingletonUtilities::Instance()->DisplayImage("imgBackProjection", &imgBackProjection);

	cv::waitKey(0);
	return(0);
}

void SetImagesNamesAndQualityFocus(std::vector<std::vector<std::string>>* namesAndQuality)
{
	std::vector<std::string> tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Focus/good");
	size_t sizeGood = tempVector.size();
	for (int i = 0; i < sizeGood; i++)
	{
		namesAndQuality->push_back({ "images/Focus/good/"+tempVector[i], "good" });
	}
	tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Focus/normal");
	size_t sizeNormal = tempVector.size();
	for (int i = 0; i < sizeNormal; i++)
	{
		namesAndQuality->push_back({ "images/Focus/normal/"+tempVector[i], "normal" });
	}

	tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Focus/bad");
	size_t sizeBad = tempVector.size();
	for (int i = 0; i < sizeBad; i++)
	{
		namesAndQuality->push_back({ "images/Focus/bad/"+tempVector[i], "bad" });
	}
}

void SetImagesNamesAndContrast(std::vector<std::vector<std::string>>* namesAndContrast)
{
	std::vector<std::string> tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Contrast/low");
	size_t sizeGood = tempVector.size();
	for (int i = 0; i < sizeGood; i++)
	{
		namesAndContrast->push_back({ "images/Contrast/low/" + tempVector[i], "low" });
	}
	tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Contrast/high");
	size_t sizeNormal = tempVector.size();
	for (int i = 0; i < sizeNormal; i++)
	{
		namesAndContrast->push_back({ "images/Contrast/high/" + tempVector[i], "high" });
	}
}