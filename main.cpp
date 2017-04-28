#include "FocusAssessment.h"
#include "SingletonUtilities.h"
#include "ColourAssessment.h"
#include "IlluminationAssessment.h"

void SetImagesNamesAndQuality(std::vector<std::vector<std::string>>*);
int main()
{
	std::vector<std::vector<std::string>> namesAndQuality;
	SetImagesNamesAndQuality(&namesAndQuality);

	// FOR DEBUGGING
	/*std::cout << "Images paths are:\n";
	for (int i = 0; i < namesAndQuality.size(); i++)
	{
		std::cout << namesAndQuality[i][o.path] << " " << namesAndQuality[i][o.quality] << std::endl;
	}
	std::cout << std::endl;*/

	// This constructor set classifier data automatically
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
	
	ColourAssessment* objColourAssessment = new ColourAssessment(namesAndQuality);
	std::vector<float> colourQuality = objColourAssessment->GetColourQuality("images/image_good.jpg");
	delete objColourAssessment;

	IlluminationAssessment* objIlluminationAssessment = new IlluminationAssessment(namesAndQuality);
	std::vector<float> illuminationQuality = objIlluminationAssessment->GetIlluminationQuality("images/image_good.jpg");
	delete objIlluminationAssessment;
	
	std::cout << "Quality measures of image are:" << std::endl;
	for (int i = 0; i < 3; i++)
	{
		std::cout << " " << focusQuality[i];
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

void SetImagesNamesAndQuality(std::vector<std::vector<std::string>>* namesAndQuality)
{
	std::vector<std::string> tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/good");
	size_t sizeGood = tempVector.size();
	for (int i = 0; i < sizeGood; i++)
	{
		namesAndQuality->push_back({ "images/good/"+tempVector[i], "good" });
	}


	tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/normal");
	size_t sizeNormal = tempVector.size();
	for (int i = 0; i < sizeNormal; i++)
	{
		namesAndQuality->push_back({ "images/normal/"+tempVector[i], "normal" });
	}

	tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/bad");
	size_t sizeBad = tempVector.size();
	for (int i = 0; i < sizeBad; i++)
	{
		namesAndQuality->push_back({ "images/bad/"+tempVector[i], "bad" });
	}
}