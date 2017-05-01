#include "FocusAssessment.h"
#include "SingletonUtilities.h"
#include "ColourAssessment.h"
#include "IlluminationAssessment.h"
#include "ContrastAssesment.h"

void SetImagesNamesAndQualityFocus(std::vector<std::vector<std::string>>*);

int main()
{
	std::vector<std::vector<std::string>> namesAndQuality;
	SetImagesNamesAndQualityFocus(&namesAndQuality);

	// This constructor sets classifier data automatically
	ContrastAssesment* objContrastAssessment = new ContrastAssesment(namesAndQuality);
	std::vector<float> contrastQuality = objContrastAssessment->GetContrastQuality("images/image_good.jpg");
	delete objContrastAssessment;

	std::cout << "Contrast measures of image are:";
	for (int i = 0; i < 4; i++)
	{
		std::cout << " " << contrastQuality[i];
	}
	std::cout << "\nContrast quality of image is ";
	contrastQuality[4] == o.good ? printf("good") : contrastQuality[4] == o.normal ? printf("normal") : printf("bad");
	std::cout << std::endl;

	// FOR DEBUGGING
	/*std::cout << "Images paths are:\n";
	for (int i = 0; i < namesAndQuality.size(); i++)
	{
		std::cout << namesAndQuality[i][o.path] << " " << namesAndQuality[i][o.quality] << std::endl;
	}
	std::cout << std::endl;*/

	// This constructor sets classifier data automatically
	std::cout << "start focus\n";
	FocusAssessment* objFocusAssessment = new FocusAssessment(namesAndQuality);
	std::vector<float> focusQuality = objFocusAssessment->GetFocusQuality("images/image_bad.jpg");
	delete objFocusAssessment;
	
	std::cout << "Focus measures of image are:";
	for (int i = 0; i < 3; i++)
	{
		std::cout << " " << focusQuality[i];
	}
	std::cout << "\nFocus quality of image is ";
	focusQuality[3] == o.good ? printf("good") : focusQuality[3] == o.normal ? printf("normal") : printf("bad");
	std::cout << std::endl;
	
	std::cout << "start color\n";
	std::vector<std::string> imageNamesColour = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Colour");
	ColourAssessment* objColourAssessment = new ColourAssessment(imageNamesColour);
	std::vector<float> colourQuality = objColourAssessment->GetColourQuality("images/image3.jpg", 0);
	std::cout << "quality of color mostly resembles: " << objColourAssessment->getColourQuality(colourQuality);
	delete objColourAssessment;

	std::cout << "\nstart ill\n";
	std::vector<std::string> imageNamesIllumination = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/Illumination");
	IlluminationAssessment* objIlluminationAssessment = new IlluminationAssessment(imageNamesIllumination);
	std::vector<float> illuminationQuality = objIlluminationAssessment->GetIlluminationQuality("images/low.jpg");
	std::cout << "quality of illumination mostly resembles: " << objIlluminationAssessment->getIlluminationQuality(illuminationQuality);
	delete objIlluminationAssessment;
	
	std::cout << "\nQuality measures of image are:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
//		std::cout << " " << contrastQuality[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < 3; i++)
	{
//		std::cout << " " << focusQuality[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < 3; i++)
	{
		std::cout << " " << colourQuality[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << " " << illuminationQuality[i];
	}
	std::cout << std::endl;

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